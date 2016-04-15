#include "VDTexture.h"

#include "cinder/gl/Texture.h"
#include "cinder/Xml.h"

using namespace ci;
using namespace ci::app;

namespace VideoDromm {
	VDTexture::VDTexture(TextureType aType)
		: mFilePathOrText("")
		, mName("")
		, mTopDown(true)
		, mFlipV(false)
		, mFlipH(true)
		, mWidth(640)
		, mHeight(480)
	{

		if (mName.length() == 0) {
			mName = mFilePathOrText;
		}
		// init the texture whatever happens next
		if (mFilePathOrText.length() > 0) {
			mTexture = ci::gl::Texture::create(ci::loadImage(mFilePathOrText), ci::gl::Texture::Format().loadTopDown(mTopDown));
		}
		else {
			mTexture = ci::gl::Texture::create(mWidth, mHeight, ci::gl::Texture::Format().loadTopDown(mTopDown));
		}
	}
	VDTexture::~VDTexture(void) {

	}

	VDTextureList VDTexture::readSettings(const DataSourceRef &source)
	{
		XmlTree			doc;
		VDTextureList	vdtexturelist;

		// try to load the specified xml file
		try { doc = XmlTree(source); }
		catch (...) { return vdtexturelist; }

		// check if this is a valid file 
		bool isOK = doc.hasChild("textures");
		if (!isOK) return vdtexturelist;

		//
		if (isOK) {

			XmlTree texturesXml = doc.getChild("textures");

			// iterate textures
			for (XmlTree::ConstIter child = texturesXml.begin("texture"); child != texturesXml.end(); ++child) {

				// create texture of the correct type
				std::string texturetype = child->getAttributeValue<std::string>("texturetype", "unknown");
				std::string filepath = child->getAttributeValue<std::string>("filepath", "");
				if (texturetype == "image") {
					TextureImageRef t(new TextureImage(filepath));
					// TODO t->fromXml(child->);
					t->mType = IMAGE;
					vdtexturelist.push_back(t);
				}
				else if (texturetype == "text") {
					TextureTextRef t(new TextureText(filepath));
					vdtexturelist.push_back(t);
				}
				else if (texturetype == "movie") {
					TextureMovieRef t(new TextureMovie(filepath));
					vdtexturelist.push_back(t);
				}
			}
		}

		return vdtexturelist;
	}

	void VDTexture::writeSettings(const VDTextureList &vdtexturelist, const ci::DataTargetRef &target) {

		// create config document and root <textures>
		XmlTree			doc;
		doc.setTag("textures");
		doc.setAttribute("version", "1.0");

		// 
		for (unsigned i = 0; i < vdtexturelist.size(); ++i) {
			// create <texture>
			XmlTree			texture;
			texture.setTag("texture");
			texture.setAttribute("id", i + 1);

			switch (vdtexturelist[i]->mType) {

			case IMAGE: texture.setAttribute("texturetype", "image"); break;
			case TEXT: texture.setAttribute("texturetype", "text"); break;
			case MOVIE: texture.setAttribute("texturetype", "movie"); break;
			default: texture.setAttribute("texturetype", "unknown"); break;
			}
			// add texture doc
			texture.setAttribute("filepath", vdtexturelist[i]->mFilePathOrText); 
			doc.push_back(texture);
		}

		// write file
		doc.write(target);
	}

	int VDTexture::getTextureWidth() {
		return mWidth;
	};

	int VDTexture::getTextureHeight() {
		return mHeight;
	};

	ci::ivec2 VDTexture::getSize() {
		return mTexture->getSize();
	}

	ci::Area VDTexture::getBounds() {
		return mTexture->getBounds();
	}

	GLuint VDTexture::getId() {
		return mTexture->getId();
	}

	std::string VDTexture::getName(){
		return mName;
	}

	ci::gl::TextureRef VDTexture::getTexture() {
		return mTexture;
	}
	// --------- child classes
	TextureImage::TextureImage(const std::string &filepath) {

		fs::path fullPath = getAssetPath("") / filepath;// TODO / mVDSettings->mAssetsPath
		if (fs::exists(fullPath)) {
			mFilePathOrText = filepath; // save the existing file path
			mTexture = ci::gl::Texture::create(loadImage(loadAsset(mFilePathOrText)), ci::gl::Texture::Format().loadTopDown(mTopDown));		
		}
		else {
			mTexture = ci::gl::Texture::create(mWidth, mHeight, ci::gl::Texture::Format().loadTopDown(mTopDown));
		}
	}
	/*void TextureImage::fromXml(const XmlTree &xml)
	{
		// TODO create xml loading of specific attributes for all texturs
		// retrieve attributes global to all types of texture
		//mSomething = xml.getAttributeValue<int>("so", 16);

	}*/
	ci::gl::Texture2dRef TextureImage::getTexture() {
		return mTexture;
	}
	TextureImage::~TextureImage(void) {

	}
	TextureText::TextureText(const std::string &filepath) {

	}
	ci::gl::Texture2dRef TextureText::getTexture() {
		return mTexture;
	}
	TextureText::~TextureText(void) {

	}
	TextureMovie::TextureMovie(const std::string &filepath) {

	}
	ci::gl::Texture2dRef TextureMovie::getTexture() {
		return mTexture;
	}
	TextureMovie::~TextureMovie(void) {

	}

} // namespace VideoDromm
