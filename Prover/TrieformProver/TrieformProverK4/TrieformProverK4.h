#ifndef TRIEFORM_PROVER_K4
#define TRIEFORM_PROVER_K4

#include "../../../Bitset/Bitset.h"
#include "../../../Clausifier/Trieform/Trieform.h"
#include "../../../Clausifier/TrieformFactory/TrieformFactory.h"
#include "../../LocalSolutionMemo/LocalSolutionMemo.h"
#include <memory>
#include <string>
#include <unordered_map>

using namespace std;

class TrieformProverK4 : public Trieform {
protected:
  static Cache persistentCache;

  unsigned int assumptionsSize = 0;
  LocalSolutionMemo localMemo;
  unordered_map<string, unsigned int> idMap;

  shared_ptr<Bitset> convertAssumptionsToBitset(literal_set literals);
  void updateSolutionMemo(const shared_ptr<Bitset> &assumptions,
                          Solution solution);
  bool isInHistory(vector<shared_ptr<Bitset>> history,
                   shared_ptr<Bitset> bitset);

  void makePersistence();
  void propagateLevels();

public:
  TrieformProverK4();
  ~TrieformProverK4();

  Solution prove(vector<shared_ptr<Bitset>> history, literal_set assumptions);
  Solution prove(literal_set assumptions = literal_set());
  virtual void preprocess();
  void prepareSAT(name_set extra = name_set());

  virtual shared_ptr<Trieform> create(const shared_ptr<Formula> &formula);
  virtual shared_ptr<Trieform> create(const shared_ptr<Formula> &formula,
                                      const vector<int> &newModality);
  virtual shared_ptr<Trieform> create(const vector<int> &newModality);
};

#endif