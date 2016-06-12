#ifndef CSVG_RENDERER_H
#define CSVG_RENDERER_H

#include <CMatrix2D.h>
#include <CBBox2D.h>
#include <CGenGradient.h>
#include <CFont.h>
#include <CImageLib.h>
#include <CRGBA.h>
#include <CLineDash.h>
#include <CLineCapType.h>
#include <CLineJoinType.h>
#include <CFillType.h>

class CSVGRenderer {
 public:
  CSVGRenderer();

  virtual ~CSVGRenderer();

  virtual CSVGRenderer *dup() const = 0;

  bool isAlpha() const { return alpha_; }
  void setAlpha(bool b) { alpha_ = b; }

  double opacity() const { return opacity_; }
  void setOpacity(double r) { opacity_ = r; }

  virtual void setSize(int width, int height) = 0;
  virtual void getSize(int *width, int *height) const = 0;

  virtual void setPixelRange(int width, int height) = 0;

  virtual void setDataRange(double xmin, double ymin, double xmax, double ymax) = 0;
  virtual void getDataRange(double *xmin, double *ymin, double *xmax, double *ymax) const = 0;

  virtual bool isAntiAlias() const = 0;
  virtual void setAntiAlias(bool aa) = 0;

  virtual void setEqualScale(bool eq) = 0;
  virtual void setScaleMin(bool sm) = 0;

  virtual void beginDraw() { }
  virtual void endDraw  () { }

  virtual void startDraw() { }
  virtual void stopDraw () { }

  virtual void setViewMatrix(const CMatrix2D &m) = 0;

  virtual void setBackground(const CRGBA &c) = 0;

  virtual void clear(const CRGBA &bg) = 0;

  virtual void pathInit() = 0;
  virtual void pathMoveTo(const CPoint2D &p) = 0;
  virtual void pathRMoveTo(const CPoint2D &p) = 0;
  virtual void pathLineTo(const CPoint2D &p) = 0;
  virtual void pathRLineTo(const CPoint2D &p) = 0;
  virtual void pathCurveTo(const CPoint2D &p1, const CPoint2D &p2) = 0;
  virtual void pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2) = 0;
  virtual void pathCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) = 0;
  virtual void pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) = 0;
  virtual void pathArcTo(const CPoint2D &c, double xr, double yr, double a1, double a2) = 0;
  virtual void pathText(const std::string &str) = 0;
  virtual void pathClose() = 0;

  virtual bool pathGetCurrentPoint(CPoint2D &p) = 0;

  virtual void pathStroke() = 0;
  virtual void pathFill() = 0;

  virtual void savePath(const CMatrix2D &m=CMatrix2D(CMATRIX_TYPE_IDENTITY)) = 0;

  virtual void pathClip  (CSVGRenderer *renderer=0) = 0;
  virtual void pathEoclip(CSVGRenderer *renderer=0) = 0;

  virtual void initClip() = 0;

  virtual void addClipPath(CSVGRenderer *renderer) = 0;

  virtual void pathBBox(CBBox2D &bbox) = 0;

  virtual void drawImage(const CPoint2D &p, CImagePtr image) = 0;

  virtual void setFont(CFontPtr bg) = 0;

  virtual void setStrokeColor(const CRGBA &fg) = 0;

  virtual void setLineWidth(double w) = 0;
  virtual void setLineDash(const CLineDash &dash) = 0;
  virtual void setLineCap(const CLineCapType &cap) = 0;
  virtual void setLineJoin(const CLineJoinType &join) = 0;
  virtual void setMitreLimit(double limit) = 0;

  virtual void resetFill() = 0;
  virtual void setFillType(CFillType fillType) = 0;
  virtual void setFillColor(const CRGBA &bg) = 0;
  virtual void setFillGradient(CGenGradient *g) = 0;
  virtual void setFillImage(CImagePtr image) = 0;
  virtual void setFillMatrix(const CMatrix2D &m) = 0;

  virtual void setStrokeFilled(bool b) = 0;
  virtual void setStrokeFillType(CFillType fillType) = 0;
  virtual void setStrokeFillGradient(CGenGradient *g) = 0;
  virtual void setStrokeFillImage(CImagePtr image) = 0;

  virtual void setAlign(CHAlignType halign, CVAlignType valign) = 0;

  virtual void windowToPixel(const CPoint2D &w, CPoint2D &p) = 0;
  virtual void pixelToWindow(const CPoint2D &p, CPoint2D &w) = 0;

  void lengthToPixel(const CPoint2D &w, CPoint2D &p) {
    CPoint2D o(0, 0);

    CPoint2D p1, p2;

    windowToPixel(o, p1);
    windowToPixel(w, p2);

    p = CPoint2D(fabs(p2.x - p1.x), fabs(p2.y - p1.y));
  }

  virtual void textBounds(const std::string &str, CBBox2D &bbox) = 0;

  virtual CISize2D getImageSize() const = 0;

  virtual CImagePtr getImage() const = 0;

  virtual void setImage(CSVGRenderer *renderer) = 0;
  virtual void setImage(CImagePtr image) = 0;

  //---

  virtual void addResizedImage(CSVGRenderer *src, double x, double y, double w, double h);

  virtual void addClippedImage(CSVGRenderer *src, int x, int y,
                               int px1, int py1, int px2, int py2);
  virtual void setClippedImage(CSVGRenderer *src, int px1, int py1, int px2, int py2);

  virtual void setOffsetImage(CSVGRenderer *src, int dx, int dy);

  virtual void addImage(int x, int y, CImagePtr image);

  virtual void combine(int x, int y, CSVGRenderer *r);

  virtual void gaussianBlur(CSVGRenderer *dst, CBBox2D &bbox, double stdDevX, double stdDevY);

 protected:
  bool   alpha_   { false };
  double opacity_ { 1.0 };
};

#endif
