#include <CSVGFeFunc.h>
#include <CSVG.h>

CSVGFeFunc::
CSVGFeFunc(CSVG &svg, CColorComponent component) :
 CSVGObject(svg), component_(component)
{
}

CSVGFeFunc::
CSVGFeFunc(const CSVGFeFunc &fe) :
 CSVGObject (fe),
 component_ (fe.component_),
 type_      (fe.type_),
 slope_     (fe.slope_),
 intercept_ (fe.intercept_),
 amplitude_ (fe.amplitude_),
 exponent_  (fe.exponent_ ),
 offset_    (fe.offset_   ),
 table_     (fe.table_    )
{
}

CSVGFeFunc *
CSVGFeFunc::
dup() const
{
  return new CSVGFeFunc(*this);
}

bool
CSVGFeFunc::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string         str;
  double              real;
  std::vector<double> reals;

  if      (svg_.stringOption(opt_name, opt_value, "type", str)) {
    if      (str == "identity") type_ = Type::IDENTITY;
    else if (str == "linear"  ) type_ = Type::LINEAR;
    else if (str == "gamma"   ) type_ = Type::GAMMA;
    else if (str == "table"   ) type_ = Type::TABLE;
    else if (str == "discrete") type_ = Type::DISCRETE;
    else std::cerr << "Unrecognised type " << str << std::endl;
  }
  else if (svg_.realOption(opt_name, opt_value, "slope", &real))
    slope_ = real;
  else if (svg_.realOption(opt_name, opt_value, "intercept", &real))
    intercept_ = real;
  else if (svg_.realOption(opt_name, opt_value, "amplitude", &real))
    amplitude_ = real;
  else if (svg_.realOption(opt_name, opt_value, "exponent", &real))
    exponent_ = real;
  else if (svg_.realOption(opt_name, opt_value, "offset", &real))
    offset_ = real;
  else if (svg_.realListOption(opt_name, opt_value, "tableValues", reals))
    table_ = reals;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeFunc::
draw()
{
}

CImagePtr
CSVGFeFunc::
filterImage(CImagePtr src_image)
{
  CImagePtr dst_image = src_image->dup();

  if      (getType() == Type::IDENTITY) {
  }
  else if (getType() == Type::LINEAR)
    dst_image->linearFunc(component_, getSlope(), getIntercept());
  else if (getType() == Type::GAMMA)
    dst_image->gammaFunc(component_, getAmplitude(), getExponent(), getOffset());
  else if (getType() == Type::TABLE)
    dst_image->tableFunc(component_, table_);
  else if (getType() == Type::DISCRETE)
    dst_image->discreteFunc(component_, table_);

  return dst_image;
}

void
CSVGFeFunc::
print(std::ostream &os, bool hier) const
{
  std::string name = "feFunc";

  if      (component_ == CCOLOR_COMPONENT_RED  ) name = "feFuncR";
  else if (component_ == CCOLOR_COMPONENT_GREEN) name = "feFuncG";
  else if (component_ == CCOLOR_COMPONENT_BLUE ) name = "feFuncB";
  else if (component_ == CCOLOR_COMPONENT_ALPHA) name = "feFuncA";

  if (hier) {
    os << "<" << name;

    CSVGObject::printValues(os);

    if (type_.isValid()) {
      os << " type=\"";

      if      (getType() == Type::IDENTITY) os << "identity";
      else if (getType() == Type::LINEAR  ) os << "linear";
      else if (getType() == Type::GAMMA   ) os << "gamma";
      else if (getType() == Type::TABLE   ) os << "table";
      else if (getType() == Type::DISCRETE) os << "discrete";

      os << "\"";
    }

    printNameValue(os, "slope"    , slope_);
    printNameValue(os, "intercept", intercept_);
    printNameValue(os, "amplitude", amplitude_);
    printNameValue(os, "exponent" , exponent_);
    printNameValue(os, "offset"   , offset_);

    if (! table_.empty()) {
      os << " tableValues=\"";

      int i = 0;

      for (const auto &r : table_) {
        if (i > 0) os << " ";

        os << r;

        ++i;
      }

      os << "\"";
    }

    os << "/>" << std::endl;
  }
  else
    os << name << " ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeFunc &filter)
{
  filter.print(os, false);

  return os;
}
