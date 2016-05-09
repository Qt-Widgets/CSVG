#include <CSVGImage.h>
#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CSVGUtil.h>

CSVGImage::
CSVGImage(CSVG &svg) :
 CSVGObject(svg),
 xlink_    (this)
{
}

CSVGImage::
CSVGImage(const CSVGImage &image) :
 CSVGObject(image),
 xlink_         (image.xlink_),
 x_             (image.x_),
 y_             (image.y_),
 w_             (image.w_),
 h_             (image.h_),
 preserveAspect_(image.preserveAspect_)
{
}

CSVGImage::
~CSVGImage()
{
}

CSVGImage *
CSVGImage::
dup() const
{
  return new CSVGImage(*this);
}

void
CSVGImage::
setLinkName(const std::string &str)
{
  xlink_ = CSVGXLink(this, str);
}

/* Atributes:
    <Core>
    <Conditional>
    <Style>
    <Viewport>
    <Color>
    <Opacity>
    <Graphics>
    <ColorProfile>
    <Clip>
    <Mask>
    <Filter>
    <GraphicalEvents>
    <Cursor>
    <XLinkEmbed>
    <External>
    x <Coordinate>
    y <Coordinate>
    width <Length>
    height <Length>
    preserveAspectRatio <PreserveAspectRatioSpec>
    transform <TransformList>
*/
bool
CSVGImage::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string        str;
  double             real;
  CSVGLengthValue    length;
  CSVGPreserveAspect preserveAspect;

  if      (svg_.coordOption (opt_name, opt_value, "x", &real))
    x_ = real;
  else if (svg_.coordOption (opt_name, opt_value, "y", &real))
    y_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "width", length))
    w_ = length;
  else if (svg_.lengthOption(opt_name, opt_value, "height", length))
    h_ = length;
  else if (svg_.preserveAspectOption(opt_name, opt_value, "preserveAspectRatio", preserveAspect))
    preserveAspect_ = preserveAspect;
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str))
    xlink_ = CSVGXLink(this, str);
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGImage::
getSize(CSize2D &size) const
{
  size = CSize2D(getWidth(), getHeight());

  return true;
}

void
CSVGImage::
setOrigin(const CPoint2D &pos)
{
  x_ = pos.x;
  y_ = pos.y;
}

void
CSVGImage::
setSize(const CSize2D &size)
{
  w_ = size.width;
  h_ = size.height;
}

#if 0
void
CSVGImage::
setImage(const std::string &filename)
{
  CImageFileSrc src(filename);

  CImagePtr image = CImageMgrInst->lookupImage(src);

  if (xlink_.isValid())
    xlink_.getValue().setImage(image);
}
#endif

void
CSVGImage::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  CImagePtr image = getImage();

  if (! image.isValid())
    return;

  //---

  CSVGBuffer *oldBuffer = svg_.getBuffer();

  //---

  // get bbox
  CBBox2D bbox;

  getBBox(bbox);

  // resize image to bbox
  double x1, y1, x2, y2;

  oldBuffer->windowToPixel(bbox.getXMin(), bbox.getYMin(), &x1, &y1);
  oldBuffer->windowToPixel(bbox.getXMax(), bbox.getYMax(), &x2, &y2);

  int pw = CSVGUtil::round(fabs(x2 - x1));
  int ph = CSVGUtil::round(fabs(y2 - y1));

  image->reshape(pw, ph);

  if (svg_.getDebugImage()) {
    std::string imageBufferName = "image_" + getUniqueName();

    CSVGBuffer *imageBuffer = svg_.getBuffer(imageBufferName);

    imageBuffer->setImage(image);
  }

  //---

  bool oldDrawing = oldBuffer->isDrawing();

  if (oldDrawing)
    oldBuffer->stopDraw();

  oldBuffer->addImage(x1, y1, image);

  if (oldDrawing)
    oldBuffer->startDraw();
}

CImagePtr
CSVGImage::
getImage() const
{
  if (xlink_.isValid()) {
    if (xlink_.getValue().isObject()) {
      CSVGImage *th = const_cast<CSVGImage *>(this);

      CImagePtr image = xlink_.getValue().getObject()->toImage();

      // TODO: don't cache image as could be rescaled
      th->xlink_.getValue().setObject(0);
      th->xlink_.getValue().setImage (image);
    }

    if (xlink_.getValue().isImage())
      return xlink_.getValue().getImage();
  }

  return CImagePtr();
}

bool
CSVGImage::
getBBox(CBBox2D &bbox) const
{
  if (! viewBox_.isValid())
    bbox = CBBox2D(getPosition(), getPosition() + getSizeInternal());
  else
    bbox = getViewBox();

  return true;
}

void
CSVGImage::
moveBy(const CVector2D &delta)
{
  CPoint2D p = getPosition();

  p += delta;

  x_ = p.x;
  y_ = p.y;
}

void
CSVGImage::
resizeTo(const CSize2D &size)
{
  setSize(size);
}

void
CSVGImage::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<image";

    CSVGObject::printValues(os);

    printNameValue (os, "x"     , x_);
    printNameValue (os, "y"     , y_);
    printNameLength(os, "width" , w_);
    printNameLength(os, "height", h_);

    printNamePreserveAspect(os, "preserveAspectRatio", preserveAspect_);

    printNameXLink(os, "xlink:href", xlink_);

    os << "/>" << std::endl;
  }
  else
    os << "image " << getPosition() << " " << getSizeInternal();
}

std::ostream &
operator<<(std::ostream &os, const CSVGImage &image)
{
  image.print(os, false);

  return os;
}
