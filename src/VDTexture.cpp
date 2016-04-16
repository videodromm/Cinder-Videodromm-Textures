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
				XmlTree detailsXml = child->getChild("details");

				// create texture of the correct type
				std::string texturetype = detailsXml.getAttributeValue<std::string>("texturetype", "unknown");
				//std::string filepath = detailsXml.getAttributeValue<std::string>("filepath", "");
				if (texturetype == "image") {
					TextureImageRef t(new TextureImage());
					t->fromXml(detailsXml);
					vdtexturelist.push_back(t);
				}
				else if (texturetype == "text") {
					TextureTextRef t(new TextureText());
					t->fromXml(detailsXml);
					vdtexturelist.push_back(t);
				}
				else if (texturetype == "movie") {
					TextureMovieRef t(new TextureMovie());
					t->fromXml(detailsXml);
					vdtexturelist.push_back(t);
				}
				else if (texturetype == "camera") {
					TextureCameraRef t(new TextureCamera());
					t->fromXml(detailsXml);
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
			case CAMERA: texture.setAttribute("texturetype", "camera"); break;
			default: texture.setAttribute("texturetype", "unknown"); break;
			}
			// details specific to texture type
			texture.push_back(vdtexturelist[i]->toXml());

			// add texture doc
			//texture.setAttribute("filepath", vdtexturelist[i]->mFilePathOrText);
			doc.push_back(texture);
		}

		// write file
		doc.write(target);
	}
	XmlTree	VDTexture::toXml() const
	{
		XmlTree		xml;
		xml.setTag("details");
		switch (mType) {
		case IMAGE: xml.setAttribute("texturetype", "image"); break;
		case TEXT: xml.setAttribute("texturetype", "text"); break;
		case MOVIE: xml.setAttribute("texturetype", "movie"); break;
		case CAMERA: xml.setAttribute("texturetype", "camera"); break;
		default: xml.setAttribute("texturetype", "unknown"); break;
		}
		xml.setAttribute("filepath", mFilePathOrText);
		xml.setAttribute("width", mWidth);
		xml.setAttribute("height", mHeight);

		return xml;
	}

	void VDTexture::fromXml(const XmlTree &xml)
	{

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
	TextureImage::TextureImage() {
	}
	XmlTree	TextureImage::toXml() const {
		XmlTree xml = VDTexture::toXml();

		// add attributes specific to this type of texture
		xml.setAttribute("path", getAssetPath("").string());
		return xml;
	}

	void TextureImage::fromXml(const XmlTree &xml)
	{
		VDTexture::fromXml(xml);
		// retrieve attributes specific to this type of texture
		mFilePathOrText = xml.getAttributeValue<string>("filepath", "");
		if (mFilePathOrText.length() > 0) {
			fs::path fullPath = getAssetPath("") / mFilePathOrText;// TODO / mVDSettings->mAssetsPath
			if (fs::exists(fullPath)) {
				//mFilePathOrText = filepath; // save the existing file path
				mTexture = ci::gl::Texture::create(loadImage(loadAsset(mFilePathOrText)), ci::gl::Texture::Format().loadTopDown(mTopDown));
			}
			else {
				mTexture = ci::gl::Texture::create(mWidth, mHeight, ci::gl::Texture::Format().loadTopDown(mTopDown));
			}
		}
		mType = IMAGE;
	}
	ci::gl::Texture2dRef TextureImage::getTexture() {
		return mTexture;
	}
	TextureImage::~TextureImage(void) {

	}
	TextureText::TextureText() {

	}
	void TextureText::fromXml(const XmlTree &xml)
	{
		// retrieve attributes specific to this type of texture
		mFilePathOrText = xml.getAttributeValue<string>("filepath", "");
		mType = TEXT;

	}
	XmlTree	TextureText::toXml() const {
		XmlTree xml = VDTexture::toXml();

		// add attributes specific to this type of texture
		xml.setAttribute("filepath", mFilePathOrText);
		return xml;
	}
	ci::gl::Texture2dRef TextureText::getTexture() {
		return mTexture;
	}
	TextureText::~TextureText(void) {

	}

	// TextureMovie
	TextureMovie::TextureMovie() {

	}
	void TextureMovie::fromXml(const XmlTree &xml)
	{
		// retrieve attributes specific to this type of texture
		mFilePathOrText = xml.getAttributeValue<string>("filepath", "");
		mType = MOVIE;

	}
	XmlTree	TextureMovie::toXml() const {
		XmlTree xml = VDTexture::toXml();

		// add attributes specific to this type of texture
		xml.setAttribute("filepath", mFilePathOrText);
		return xml;
	}
	ci::gl::Texture2dRef TextureMovie::getTexture() {
		return mTexture;
	}
	TextureMovie::~TextureMovie(void) {

	}

	// TextureCamera
	TextureCamera::TextureCamera() {
		mFirstCameraDeviceName = "";
		printDevices();

		try {
			mCapture = Capture::create(640, 480);
			mCapture->start();
		}
		catch (ci::Exception &exc) {
			CI_LOG_EXCEPTION("Failed to init capture ", exc);
		}
	}
	void TextureCamera::fromXml(const XmlTree &xml)
	{
		// retrieve attributes specific to this type of texture
		mFilePathOrText = xml.getAttributeValue<string>("filepath", "");
		mType = CAMERA;

	}
	XmlTree	TextureCamera::toXml() const {
		XmlTree xml = VDTexture::toXml();

		// add attributes specific to this type of texture
		xml.setAttribute("camera", mFirstCameraDeviceName);
		return xml;
	}
	ci::gl::Texture2dRef TextureCamera::getTexture() {
		if (mCapture && mCapture->checkNewFrame()) {
			if (!mTexture) {
				// Capture images come back as top-down, and it's more efficient to keep them that way
				mTexture = gl::Texture::create(*mCapture->getSurface(), gl::Texture::Format().loadTopDown());
			}
			else {
				mTexture->update(*mCapture->getSurface());
			}
		}
		return mTexture;
	}
	void TextureCamera::printDevices()
	{
		for (const auto &device : Capture::getDevices()) {
			console() << "Device: " << device->getName() << " "
#if defined( CINDER_COCOA_TOUCH )
				<< (device->isFrontFacing() ? "Front" : "Rear") << "-facing"
#endif
				<< endl;
			mFirstCameraDeviceName = device->getName();
		}
	}
	TextureCamera::~TextureCamera(void) {

	}

} // namespace VideoDromm
