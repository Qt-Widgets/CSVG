#include <CSVGPath.h>
#include <CSVGPathPart.h>
#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CSVGUtil.h>

CSVGPath::
CSVGPath(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGPath::
CSVGPath(const CSVGPath &path) :
 CSVGObject(path),
 parts_    (path.parts_)
{
}

CSVGPath *
CSVGPath::
dup() const
{
  return new CSVGPath(*this);
}

bool
CSVGPath::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  double           r;
  CSVGPathPartList parts;

  if      (svg_.pathOption(opt_name, opt_value, "d", parts))
    parts_ = parts;
  else if (svg_.realOption(opt_name, opt_value, "pathLength", &r))
    pathLength_ = r;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGPath::
moveBy(const CVector2D &d)
{
  parts_.moveBy(d);
}

bool
CSVGPath::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  bbox_.setInvalid();

  double l = parts_.getLength();

  // zero length path with line cap square or round draw square of circle shape to match line width
  if (l == 0) {
    bool drawCap = true;

    if (! svg_.isStroked())
      drawCap = false;

    if (drawCap && ! stroke_.getLineCapValid())
      drawCap = false;

    CSVGColor c = getFlatStrokeColor();
    double    r = getFlatStrokeWidth()/2;

    CSVGBuffer *buffer = svg_.getCurrentBuffer();

    CPoint2D p;

    if (drawCap) {
      std::vector<CPoint2D> points;
      std::vector<double>   angles;

      buffer->pathInit();

      parts_.draw(buffer, points, angles);

      if (! points.empty())
        p = points[0];
      else
        drawCap = false;
    }

    if (drawCap) {
      buffer->resetFill();

      buffer->setFillColor(c.rgba());

      if      (stroke_.getLineCap() == LINE_CAP_TYPE_SQUARE) {
        buffer->pathInit();

        CBBox2D bbox(p.x - r, p.y - r, p.x + r, p.y + r);

        buffer->pathMoveTo(bbox.getXMin(), bbox.getYMin());
        buffer->pathLineTo(bbox.getXMax(), bbox.getYMin());
        buffer->pathLineTo(bbox.getXMax(), bbox.getYMax());
        buffer->pathLineTo(bbox.getXMin(), bbox.getYMax());

        buffer->pathClose();

        buffer->pathFill();

        return true;
      }
      else if (stroke_.getLineCap() == LINE_CAP_TYPE_ROUND) {
        buffer->pathInit();

        buffer->pathMoveTo(p.x + r, p.y);
        buffer->pathArcTo (p.x, p.y, r, r, 0,  M_PI);
        buffer->pathArcTo (p.x, p.y, r, r, M_PI, 2*M_PI);

        buffer->pathClose();

        buffer->pathFill();

        return true;
      }
    }

    return false;
  }

  //------

  svg_.drawParts(parts_);

  return true;
}

void
CSVGPath::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<path";

    CSVGObject::printValues(os);

    printNameParts(os, "d"         , parts_);
    printNameValue(os, "pathLength", pathLength_);

    if (hasChildren()) {
      os << ">" << std::endl;

      printChildren(os, hier);

      os << "</path>" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else {
    os << "path (" << parts_ << ")";
  }
}

bool
CSVGPath::
getBBox(CBBox2D &bbox) const
{
  bool rc = true;

  if (! bbox_.isValid()) {
    if (hasViewBox())
      bbox = getViewBox();
    else {
      CSVGBuffer *currentBuffer = svg_.getCurrentBuffer();

      rc = parts_.getBBox(currentBuffer, bbox);
    }

    bbox_ = bbox;
  }
  else
    bbox = bbox_.getValue();

  return rc;
}

std::ostream &
operator<<(std::ostream &os, const CSVGPath &path)
{
  path.print(os, false);

  return os;
}
