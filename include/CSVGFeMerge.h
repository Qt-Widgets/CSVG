#ifndef CSVGFeMerge_H
#define CSVGFeMerge_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeMerge : public CSVGFilterBase {
 public:
  CSVG_OBJECT_DEF("feMerge", CSVGObjTypeId::FE_MERGE)

  CSVGFeMerge(CSVG &svg);
  CSVGFeMerge(const CSVGFeMerge &merge);

  CSVGFeMerge *dup() const override;

  std::string getFilterIn() const;
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const;
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void filterImage(CSVGBuffer *outBuffer);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeMerge &filter);

 private:
  COptString filterIn_;
  COptString filterOut_;
};

#endif
