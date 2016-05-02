#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Xml.h"
#include "cinder/Json.h"
#include "cinder/Capture.h"
#include "cinder/Log.h"
#include "cinder/Timeline.h"

#include <atomic>
#include <vector>

using namespace ci;
using namespace ci::app;
using namespace std;

namespace VideoDromm
{
	// stores the pointer to the VDTexture instance
	typedef std::shared_ptr<class VDTexture> 	VDTextureRef;
	typedef std::vector<VDTextureRef>			VDTextureList;
	// for profiling
	typedef std::chrono::high_resolution_clock Clock;

	class VDTexture: public std::enable_shared_from_this < VDTexture > {
	public:
		typedef enum { UNKNOWN, IMAGE, IMAGESEQUENCE, MOVIE, CAMERA } TextureType;
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
		//!
		virtual void					fromXml(const ci::XmlTree &xml);
		//!
		virtual XmlTree					toXml() const;
		//! read a xml file and pass back a vector of VDTextures
		static VDTextureList			readSettings(const ci::DataSourceRef &source);
		//! write a xml file
		static void						writeSettings(const VDTextureList &vdtexturelist, const ci::DataTargetRef &target);
		void							loadImageFromFileFullPath(string apath);
		string							getStatus() { return mStatus; };
	protected:
		std::string						mName;
		bool							mFlipV;
		bool							mFlipH;
		TextureType						mType;
		std::string						mPath;
		bool							mTopDown;
		int								mWidth;
		int								mHeight;
		std::string						mStatus;

		//! Texture
		ci::gl::Texture2dRef			mTexture;
	};

	// ---- TextureImage ------------------------------------------------
	typedef std::shared_ptr<class TextureImage>	TextureImageRef;

	class TextureImage
		: public VDTexture {
	public:
		//
		static TextureImageRef create() { return std::make_shared<TextureImage>(); }
		//!
		void					fromXml(const XmlTree &xml) override;
		//!
		virtual	XmlTree			toXml() const override;
	public:
		TextureImage();
		virtual ~TextureImage(void);

		//! returns a shared pointer
		TextureImageRef	getPtr() { return std::static_pointer_cast<TextureImage>(shared_from_this()); }
	protected:
		//! 
		virtual ci::gl::Texture2dRef	getTexture() override;
	private:
		//ci::gl::Texture2dRef	mTexture;

	};

	// ---- TextureImageSequence ------------------------------------------------
	typedef std::shared_ptr<class TextureImageSequence>	TextureImageSequenceRef;

	class TextureImageSequence
		: public VDTexture {
	public:
		//
		static TextureImageSequenceRef	create() { return std::make_shared<TextureImageSequence>(); }
		//!
		void					fromXml(const XmlTree &xml) override;
		//!
		virtual	XmlTree			toXml() const override;
		TextureImageSequence();
		virtual ~TextureImageSequence(void);

		//! returns a shared pointer 
		TextureImageSequenceRef	getPtr() { return std::static_pointer_cast<TextureImageSequence>(shared_from_this()); }
		void						playPauseSequence();
		void						syncToBeat();
		void						stopSequence();
		void						toggleLoadingFromDisk();
		void						stopLoading();
		int							getPlayheadPosition();
		void						setPlayheadPosition(int position);

		int							getSpeed();
		void						setSpeed(int speed);
		void						reverseSequence();
		bool						isLoadingFromDisk();
		bool						isValid(){ return mFramesLoaded > 0; };
		int							getMaxFrame();	
	protected:
		//! 
		virtual ci::gl::Texture2dRef	getTexture() override;

	private:
		int							playheadFrameInc;
		void						loadNextImageFromDisk();
		void						updateSequence();
		bool						mIsSequence;
		string						mFolder;
		string						mPrefix;
		string						mExt;
		int							mNumberOfDigits;
		int							mNextIndexFrameToTry;
		int							mCurrentLoadedFrame;
		int							mFramesLoaded;
		int							mPlayheadPosition;
		bool						mLoadingPaused;
		bool						mLoadingFilesComplete;
		bool						mPlaying;
		int							mSpeed;
		bool						mSyncToBeat;
		vector<ci::gl::TextureRef>	mSequenceTextures;
	};

	// ---- TextureMovie ------------------------------------------------
	typedef std::shared_ptr<class TextureMovie>	TextureMovieRef;

	class TextureMovie
		: public VDTexture {
	public:
		//
		static TextureMovieRef create() { return std::make_shared<TextureMovie>(); }
		//!
		void				fromXml(const XmlTree &xml) override;
		//!
		virtual	XmlTree	toXml() const override;

	public:
		TextureMovie();
		virtual ~TextureMovie(void);

		//! returns a shared pointer 
		TextureMovieRef	getPtr() { return std::static_pointer_cast<TextureMovie>(shared_from_this()); }
	protected:
		//! 
		virtual ci::gl::Texture2dRef	getTexture() override;
	private:
		//ci::gl::Texture2dRef	mTexture;
	};
	// ---- TextureCamera ------------------------------------------------

#if (defined(  CINDER_MSW) ) || (defined( CINDER_MAC ))
	typedef std::shared_ptr<class TextureCamera>	TextureCameraRef;

	class TextureCamera
		: public VDTexture {
	public:
		//
		static TextureCameraRef create() { return std::make_shared<TextureCamera>(); }
		//!
		void				fromXml(const XmlTree &xml) override;
		//!
		virtual	XmlTree	toXml() const override;

	public:
		TextureCamera();
		virtual ~TextureCamera(void);

		//! returns a shared pointer 
		TextureCameraRef	getPtr() { return std::static_pointer_cast<TextureCamera>(shared_from_this()); }
	protected:
		//! 
		virtual ci::gl::Texture2dRef	getTexture() override;
	private:
		void printDevices();
		string					mFirstCameraDeviceName;
		CaptureRef				mCapture;
		//ci::gl::Texture2dRef	mTexture;
	};
#else

#endif
}
