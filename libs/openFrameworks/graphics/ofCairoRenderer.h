#pragma once

#include "cairo.h"
#include "ofGraphicsBaseTypes.h"
// MARK: Optimization opportunity in ofPath, ofPixels pointer.
#include "ofPath.h"
#include "ofPixels.h" // MARK: ofPixels imageBuffer;
#include "of3dGraphics.h"

#include <deque>
#include <stack>

class ofCairoRenderer: public ofBaseRenderer{
public:
	ofCairoRenderer();
	~ofCairoRenderer();

	static const std::string TYPE;
	const std::string & getType(){ return TYPE; }

	enum Type{
		PDF,
		SVG,
		IMAGE,
		FROM_FILE_EXTENSION
	};

	void setup(const of::filesystem::path & filename, Type type=ofCairoRenderer::FROM_FILE_EXTENSION, bool multiPage=true, bool b3D=false, ofRectangle outputsize = ofRectangle(0,0,0,0));
	void setupMemoryOnly(Type _type, bool multiPage=true, bool b3D=false, ofRectangle viewport = ofRectangle(0,0,0,0));
	void close();
	void flush();

	void startRender();
	void finishRender();

	using ofBaseRenderer::draw;
	void draw(const ofPath & shape) const;
	void draw(const ofPath::Command & path) const;
	void draw(const ofPolyline & poly) const;
	void draw(const ofMesh & vertexData, ofPolyRenderMode mode, bool useColors, bool useTextures, bool useNormals) const;
    void draw(const of3dPrimitive& model, ofPolyRenderMode renderType ) const;
    void draw(const ofNode& node) const;
	void draw(const std::vector<glm::vec3> & vertexData, ofPrimitiveMode drawMode) const;
	void draw(const ofImage & img, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
	void draw(const ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
	void draw(const ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
	void draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const;
	ofPath & getPath();

	//--------------------------------------------
	// transformations
	void pushView();
	void popView();

	// setup matrices and viewport (upto you to push and pop view before and after)
	// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
	// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
	void viewport(ofRectangle viewport);
	void viewport(float x = 0, float y = 0, float width = -1, float height = -1, bool invertY = true);
	void setupScreenPerspective(float width = -1, float height = -1, float fov = 60, float nearDist = 0, float farDist = 0);
	void setupScreenOrtho(float width = -1, float height = -1, float nearDist = -1, float farDist = 1);
	ofRectangle getCurrentViewport() const;
	ofRectangle getNativeViewport() const;
	int getViewportWidth() const;
	int getViewportHeight() const;

	void setOrientation(ofOrientation orientation, bool vFlip);
	bool isVFlipped() const;
	void loadViewMatrix(const glm::mat4 & m);
	void multViewMatrix(const glm::mat4 & m);
	glm::mat4 getCurrentViewMatrix() const;
	glm::mat4 getCurrentNormalMatrix() const;
	glm::mat4 getCurrentOrientationMatrix() const;
	void setCircleResolution(int);


	void setCoordHandedness(ofHandednessType handedness);
	ofHandednessType getCoordHandedness() const;

	// drawing modes
	void setRectMode(ofRectMode mode);
	ofRectMode getRectMode();
	void setFillMode(ofFillFlag fill);
	ofFillFlag getFillMode();
	void setLineWidth(float lineWidth);
	void setDepthTest(bool depthTest);
	void setBlendMode(ofBlendMode blendMode);
	void setLineSmoothing(bool smooth);
	void enableAntiAliasing();
	void disableAntiAliasing();

	//our openGL wrappers
	void pushMatrix();
	void popMatrix();
	glm::mat4 getCurrentMatrix(ofMatrixMode matrixMode_) const;
	void translate(float x, float y, float z = 0);
	void translate(const glm::vec3 & p);
	void scale(float xAmnt, float yAmnt, float zAmnt = 1);
	void rotateRad(float radians, float vecX, float vecY, float vecZ);
	void rotateXRad(float radians);
	void rotateYRad(float radians);
	void rotateZRad(float radians);
	void rotateRad(float radians);
	void matrixMode(ofMatrixMode mode);
	void loadIdentityMatrix (void);
	void loadMatrix (const glm::mat4 & m);
	void loadMatrix (const float * m);
	void multMatrix (const glm::mat4 & m);
	void multMatrix (const float * m);

	// screen coordinate things / default gl values
	void setupGraphicDefaults();
	void setupScreen();

	// color options
	void setColor(float r, float g, float b); // 0-1
	void setColor(float r, float g, float b, float a); // 0-1
	void setColor(const ofFloatColor & color);
	void setColor(const ofFloatColor & color, float _a);
	void setColor(float gray); // new set a color as grayscale with one argument
	void setHexColor( int hexColor ); // hex, like web 0xFF0033;

	// bg color
	void setBackgroundColor(const ofFloatColor & c);
	ofFloatColor getBackgroundColor();
	void background(const ofFloatColor & c);
	void background(float brightness);
	void background(int hexColor, int _a=255);
	void background(float r, float g, float b, float a=1.f);

	void setBackgroundAuto(bool bManual);		// default is true
	bool getBackgroundAuto();

	void clear();
	void clear(float r, float g, float b, float a=0);
	void clear(float brightness, float a=0);
	void clearAlpha();

	void setBitmapTextMode(ofDrawBitmapMode mode);

	ofStyle getStyle() const;
	void pushStyle();
	void popStyle();
	void setStyle(const ofStyle & style);
	void setCurveResolution(int resolution);
	void setPolyMode(ofPolyWindingMode mode);

	// drawing
	void drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const;
	void drawRectangle(float x, float y, float z, float w, float h) const;
	void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const;
	void drawCircle(float x, float y, float z, float radius) const;
	void drawEllipse(float x, float y, float z, float width, float height) const;
	void drawString(std::string text, float x, float y, float z) const;
	void drawString(const ofTrueTypeFont & font, std::string text, float x, float y) const;

	// cairo specifics
	cairo_t * getCairoContext();
	cairo_surface_t * getCairoSurface();
	ofPixels & getImageSurfacePixels();
	ofBuffer & getContentBuffer();


	virtual void bind(const ofCamera & camera, const ofRectangle & viewport){}
	virtual void unbind(const ofCamera & camera){}

	const of3dGraphics & get3dGraphics() const;
	of3dGraphics & get3dGraphics();

private:
	glm::vec3 transform(glm::vec3 vec) const;
	static _cairo_status stream_function(void *closure,const unsigned char *data, unsigned int length);
	void draw(const ofPixels & img, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;

	mutable std::deque<glm::vec3> curvePoints;
	cairo_t * cr;
	cairo_surface_t * surface;
	bool bBackgroundAuto;

	std::stack<cairo_matrix_t> matrixStack;

	Type type;
	int page;
	bool multiPage;

	// 3d transformation
	bool b3D;
	of3dGraphics graphics3d;

	glm::mat4 projection;
	glm::mat4 modelView;
	ofRectangle viewportRect, originalViewport;

	std::stack<glm::mat4> projectionStack;
	std::stack<glm::mat4> modelViewStack;
	std::stack<ofRectangle> viewportStack;
	
	ofMatrixMode currentMatrixMode;

	std::vector<glm::vec3> sphereVerts;
	std::vector<glm::vec3> spherePoints;

	of::filesystem::path filename;
	ofBuffer streamBuffer;
	ofPixels imageBuffer;

	ofStyle currentStyle;
	std::deque <ofStyle> styleHistory;
	ofPath path;
};
