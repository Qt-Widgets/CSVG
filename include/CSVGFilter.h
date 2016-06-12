#ifndef CSVG_FILTER_H
#define CSVG_FILTER_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGFilter : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("filter", CSVGObjTypeId::FILTER)

  CSVGFilter(CSVG &svg);
  CSVGFilter(const CSVGFilter &filter);

  CSVGFilter *dup() const override;

  CSVGCoordUnits getUnits() const {
    return filterUnits_.getValue(CSVGCoordUnits::OBJECT_BBOX); }
  void setUnits(CSVGCoordUnits units) { filterUnits_ = units; }

  CSVGCoordUnits getPrimitiveUnits() const {
    return primitiveUnits_.getValue(CSVGCoordUnits::USER_SPACE); }
  void setPrimitiveUnits(CSVGCoordUnits units) { primitiveUnits_ = units; }

  bool hasX() const { return x_.isValid(); }
  CScreenUnits getX(const CScreenUnits &x=CScreenUnits()) const { return x_.getValue(x); }
  void setX(const CScreenUnits &x) { x_ = x; }

  bool hasY() const { return y_.isValid(); }
  CScreenUnits getY(const CScreenUnits &y=CScreenUnits()) const { return y_.getValue(y); }
  void setY(const CScreenUnits &y) { y_ = y; }

  bool hasWidth() const { return width_.isValid(); }
  CScreenUnits getWidth(const CScreenUnits &w=CScreenUnits()) const {
    return width_.getValue(w); }
  void setWidth(const CScreenUnits &w) { width_ = w; }

  bool hasHeight() const { return height_.isValid(); }
  CScreenUnits getHeight(const CScreenUnits &h=CScreenUnits()) const {
    return height_.getValue(h); }
  void setHeight(const CScreenUnits &h) { height_ = h; }

  const CSVGXLink &xlink() const { return xlink_.getValue(); }

  std::string getLinkName() const { return (xlink_.isValid() ? xlink_.getValue().str() : ""); }
  void setLinkName(const std::string &str);

  bool processOption(const std::string &name, const std::string &value) override;

  CSVGObject *getObject() const { return object_; }
  void setObject(CSVGObject *object) { object_ = object; }

  const CBBox2D &getContentsBBox() const { return contentsBBox_; }
  void setContentsBBox(const CBBox2D &bbox) { contentsBBox_ = bbox; }

  const std::string &getLastFilterName() const { return lastFilterName_; }
  void setLastFilterName(const std::string &v) { lastFilterName_ = v; }

  bool getRegion(CBBox2D &bbox) const;
  bool getRegion(CSVGObject *obj, CBBox2D &bbox) const;

  void initDraw(CSVGBuffer *buffer);
  void termDraw(CSVGBuffer *buffer);

  bool draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFilter &filter);

 private:
  CSVGFilter &operator=(const CSVGFilter &rhs);

 private:
  CSVGObject*              object_ { 0 };
  COptValT<CSVGCoordUnits> filterUnits_;
  COptValT<CSVGCoordUnits> primitiveUnits_;
  COptValT<CScreenUnits>   x_;
  COptValT<CScreenUnits>   y_;
  COptValT<CScreenUnits>   width_;
  COptValT<CScreenUnits>   height_;
  COptString               filterRes_;
  COptValT<CSVGXLink>      xlink_;
  CBBox2D                  contentsBBox_;
  bool                     oldDrawing_ { false };
  std::string              lastFilterName_;
};

#endif
