#pragma once

#include "cinder/gl/gl.h"

#include <atomic>
#include <vector>

// forward declarations
namespace cinder {
	class XmlTree;

	namespace app {
		class	KeyEvent;
		class	MouseEvent;
	}

	namespace gl {
		class	Texture2d;
		typedef	std::shared_ptr<Texture2d>	Texture2dRef;
	}
}

namespace VideoDromm
{
	// stores the pointer to the VDTexture instance
	typedef std::shared_ptr<class VDTexture> 	VDTextureRef;
	typedef std::vector<VDTextureRef>			VDTextureList;

	class VDTexture: public std::enable_shared_from_this < VDTexture > {
	public:
		typedef enum { UNKNOWN, IMAGE, TEXT, MOVIE } TextureType;
	public:
		VDTexture( TextureType aType = UNKNOWN);
		virtual ~VDTexture( void );

		virtual ci::gl::Texture2dRef	getTexture();
		//! returns a shared pointer to this input texture
		VDTextureRef					getPtr() { return shared_from_this(); }
		ci::ivec2						getSize();
		ci::Area						getBounds();
		GLuint							getId();
		//! returns the type
		TextureType						getType() { return mType; };
		std::string						getName();
		bool							isFlipH() { return mFlipH; };
		bool							isFlipV() { return mFlipV; };
		int								getTextureWidth();
		int								getTextureHeight();
		//! read a xml file and pass back a vector of VDTextures
		static VDTextureList			readSettings(const ci::DataSourceRef &source);
		//! write a xml file
		static void						writeSettings(const VDTextureList &vdtexturelist, const ci::DataTargetRef &target);

	protected:
		std::string						mName;
		bool							mFlipV;
		bool							mFlipH;
		TextureType						mType;
		std::string						mFilePathOrText;
		bool							mTopDown;
		int								mWidth;
		int								mHeight;
		//! Texture
		ci::gl::Texture2dRef			mTexture;
	};

	// ---- TextureImage ------------------------------------------------
	typedef std::shared_ptr<class TextureImage>	TextureImageRef;

	class TextureImage
		: public VDTexture {
	public:
		//
		static TextureImageRef create(const std::string &filepath) { return std::make_shared<TextureImage>(filepath); }

	public:
		TextureImage(const std::string &filepath);
		virtual ~TextureImage(void);

		//! returns a shared pointer
		TextureImageRef	getPtr() { return std::static_pointer_cast<TextureImage>(shared_from_this()); }
	protected:
		//! 
		virtual ci::gl::Texture2dRef	getTexture() override;
	private:
		ci::gl::Texture2dRef	mTexture;

	};

	// ---- TextureText ------------------------------------------------
	typedef std::shared_ptr<class TextureText>	TextureTextRef;

	class TextureText
		: public VDTexture {
	public:
		//
		static TextureTextRef create(const std::string &filepath) { return std::make_shared<TextureText>(filepath); }

	public:
		TextureText(const std::string &filepath);
		virtual ~TextureText(void);

		//! returns a shared pointer 
		TextureTextRef	getPtr() { return std::static_pointer_cast<TextureText>(shared_from_this()); }
	protected:
		//! 
		virtual ci::gl::Texture2dRef	getTexture() override;
	private:
		ci::gl::Texture2dRef	mTexture;

	};

	// ---- TextureMovie ------------------------------------------------
	typedef std::shared_ptr<class TextureMovie>	TextureMovieRef;

	class TextureMovie
		: public VDTexture {
	public:
		//
		static TextureMovieRef create(const std::string &filepath) { return std::make_shared<TextureMovie>(filepath); }

	public:
		TextureMovie(const std::string &filepath);
		virtual ~TextureMovie(void);

		//! returns a shared pointer 
		TextureMovieRef	getPtr() { return std::static_pointer_cast<TextureMovie>(shared_from_this()); }
	protected:
		//! 
		virtual ci::gl::Texture2dRef	getTexture() override;
	private:
		ci::gl::Texture2dRef	mTexture;
	};
}
