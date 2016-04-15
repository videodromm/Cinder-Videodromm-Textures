#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "VDTexture.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace VideoDromm;

class VDApp : public App {

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


void VDApp::setup()
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

void VDApp::update()
{

}
void VDApp::cleanup()
{

	// save warp settings
	VDTexture::writeSettings(mTexs, writeFile(mTexturesFilepath));

	quit();
}
void VDApp::mouseDown(MouseEvent event)
{
    
}

void VDApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
	gl::draw(mTexs[0]->getTexture());
}


CINDER_APP(VDApp, RendererGl)
