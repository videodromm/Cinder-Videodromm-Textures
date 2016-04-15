#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "VDTexture.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace VideoDromm;

class _TBOX_PREFIX_App : public App {

public:

	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
	void cleanup() override;
private:
	VDTextureList				mTexs;
	fs::path					mTexturesFilepath;


};


void _TBOX_PREFIX_App::setup()
{
	// initialize 
	mTexturesFilepath = getAssetPath("") / "textures.xml";
	if (fs::exists(mTexturesFilepath)) {
		// load textures from file if one exists
		mTexs = VDTexture::readSettings(loadFile(mTexturesFilepath));
	}
	else {
		// otherwise create a texture from scratch
		mTexs.push_back(TextureImage::create());

	}
}

void _TBOX_PREFIX_App::update()
{

}
void _TBOX_PREFIX_App::cleanup()
{

	// save warp settings
	VDTexture::writeSettings(mTexs, writeFile(mTexturesFilepath));

	quit();
}
void _TBOX_PREFIX_App::mouseDown(MouseEvent event)
{
    
}

void _TBOX_PREFIX_App::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
	gl::draw(mTexs[0]->getTexture());
}


CINDER_APP(_TBOX_PREFIX_App, RendererGl)
