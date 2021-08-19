#include "GenCoBuchiAutomaton.h"

/*
 * Abstract function converting the automaton to string.
 * @param stateStr Function converting a state to string
 * @param symStr Function converting a symbol to string
 * @return String representation of the automaton
 */
template<typename State, typename Symbol>
std::string GeneralizedCoBuchiAutomaton<State,Symbol>::toStringWith(std::function<std::string(State)>& stateStr,  std::function<std::string(Symbol)>& symStr){
  std::string str = "";
  for (auto p : this->initials)
    str += stateStr(p) + "\n";
  for (auto p : this->trans)
  {
    for(auto d : p.second)
      str += symStr(p.first.second) + "," + stateStr(p.first.first)
        + "->" + stateStr(d) + "\n";
  }
  for (auto it = this->finals.begin(); it != this->finals.end(); it++){
    for(State p : it->second)
        str += stateStr(p) + "\n";
    str += "---\n"; // divider between accepting sets
  }

  if(str.back() == '\n')
    str.pop_back();
  return str;
}

/*
 * Abstract function converting the automaton to graphwiz format.
 * @param stateStr Function converting a state to string
 * @param symStr Function converting a symbol to string
 * @return Graphwiz representation of the automaton
 */
template <typename State, typename Symbol>
std::string GeneralizedCoBuchiAutomaton<State, Symbol>::toGraphwizWith(std::function<std::string(State)>& stateStr,
  std::function<std::string(Symbol)>& symStr)
{
  std::string str = "digraph \" Automaton \" { rankdir=LR;\n { rank = LR }\n";
  str += "node [shape = doublecircle];\n";
  for (auto st : this->states){
      std::string acc = "";
      for (auto it = this->finals.begin(); it != this->finals.end(); it++){
          if (it->second.find(st) != it->second.end()){
            if (acc != "")
                acc += ",";
            acc += stateStr(st);   
          }
      }
      if (acc != "")
        str += "\"" + stateStr(st) + "\" [label = \"" + stateStr(st) + " {" + acc + "}\"] " + "\n";
  }
  str += "node [shape = circle];";
  for(auto st : this->states)
    str += "\"" + stateStr(st) + "\"\n";
  str += "\"init0\";\n";
  for (auto p : this->initials)
    str += "\"init0\" -> \"" + stateStr(p) + "\"\n";
  for (auto p : this->trans)
  {
    for(auto d : p.second)
      str +=  "\"" + stateStr(p.first.first) + "\" -> \"" + stateStr(d) +
        + "\" [label = \"" + symStr(p.first.second) + "\"];\n";
  }
  str += "}\n";
  return str;
}

template<typename State, typename Symbol>
std::string GeneralizedCoBuchiAutomaton<State,Symbol>::toGffWith(std::function<std::string(State)>& stateStr,  std::function<std::string(Symbol)>& symStr){
    int tid = 0;
    std::string str = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
    str += "<structure label-on=\"transition\" type=\"fa\">\n";
    str += "<alphabet type=\"classical\">\n";
    for(auto s : this->getAlphabet())
      str += "<symbol>" + symStr(s) + "</symbol>\n";
    str += "</alphabet>\n";
  
    str += "<stateset>\n";
    for(auto st : this->states)
      str += "<state sid=\"" + stateStr(st) +  "\"></state>\n";
    str += "</stateset>\n";
  
    str += "<acc type=\"Generalized co-Buchi\">\n";
    for (auto it = this->finals.begin(); it != this->finals.end(); it++){
      str += "<AccSet>\n";
      for(auto p : it->second)
          str += "<stateID>" + stateStr(p) +  "</stateID>\n";
      str += "</AccSet>\n";
    }
    str += "</acc>\n";
  
    str += "<initialStateSet>\n";
    for(auto p : this->initials)
      str += "<stateID>" + stateStr(p) +  "</stateID>\n";
    str += "</initialStateSet>\n";
  
    str += "<transitionset>\n";
    for (auto p : this->trans)
    {
      for(auto d : p.second)
      {
        str += "<transition tid=\"" + std::to_string(tid++) + "\">\n";
        str +=  "<from>" + stateStr(p.first.first) + "</from>\n<to>" + stateStr(d) +
          + "</to>\n<read>" + symStr(p.first.second) + "</read>\n";
        str += "</transition>";
      }
    }
    str += "</transitionset>\n";
    str += "</structure>\n";
    return str;
}


/*
 * Function converting the automaton <int, int> to string.
 * @return String representation of the automaton
 */
template <>
std::string GeneralizedCoBuchiAutomaton<int, int>::toString()
{
  std::function<std::string(int)> f1 = [=] (int x) {return std::to_string(x);};
  std::function<std::string(int)> f2 = [=] (int x) {return std::to_string(x);};
  return toStringWith(f1, f2);
}


/*
 * Function converting the automaton <string, string> to string.
 * @return String representation of the automaton
 */
template <>
std::string GeneralizedCoBuchiAutomaton<std::string, std::string>::toString()
{
  std::function<std::string(std::string)> f1 = [&] (std::string x) {return x;};
  std::function<std::string(std::string)> f2 = [&] (std::string x) {return x;};
  return toStringWith(f1, f2);
}

/*
 * Function converting the automaton <tuple<int, int, bool>, int> to string.
 * @return String representation of the automaton
 */
template <>
std::string GeneralizedCoBuchiAutomaton<tuple<int, int, bool>, int>::toString()
{
  std::function<std::string(tuple<int, int, bool>)> f1 = [&] (tuple<int, int, bool> x)
  {
    return "(" + std::to_string(std::get<0>(x)) + " " +
      std::to_string(std::get<1>(x)) + " " + std::to_string(std::get<2>(x)) + ")";
  };
  std::function<std::string(int)> f2 = [=] (int x) {return std::to_string(x);};
  return toStringWith(f1, f2);
}

/*
 * Function converting the automaton <int, APSymbol> to string.
 * @return String representation of the automaton
 */
template <>
std::string GeneralizedCoBuchiAutomaton<int, APSymbol>::toString()
{
  std::function<std::string(int)> f1 = [=] (int x) {return std::to_string(x);};
  std::function<std::string(APSymbol)> f2 = [&] (APSymbol x) {return x.toString();};
  return toStringWith(f1, f2);
}


/*
 * Function converting the automaton <int, int> to graphwiz format.
 * @return Graphwiz representation of the automaton
 */
template <>
std::string GeneralizedCoBuchiAutomaton<int, int>::toGraphwiz()
{
  std::function<std::string(int)> f1 = [=] (int x) {return std::to_string(x);};
  std::function<std::string(int)> f2 = [=] (int x) {return std::to_string(x);};
  return toGraphwizWith(f1, f2);
}

/*
 * Function converting the automaton <string, string> to graphwiz format.
 * @return Graphwiz representation of the automaton
 */
template <>
std::string GeneralizedCoBuchiAutomaton<std::string, std::string>::toGraphwiz()
{
  std::function<std::string(std::string)> f1 = [&] (std::string x) {return x;};
  std::function<std::string(std::string)> f2 = [&] (std::string x) {return x;};
  return toGraphwizWith(f1, f2);
}


/*
 * Function converting the automaton <tuple<int, int, bool>, int> to graphwiz format.
 * @return Graphwiz representation of the automaton
 */
template <>
std::string GeneralizedCoBuchiAutomaton<tuple<int, int, bool>, int>::toGraphwiz()
{
  std::function<std::string(tuple<int, int, bool>)> f1 = [&] (tuple<int, int, bool> x)
  {
    return "(" + std::to_string(std::get<0>(x)) + " " +
      std::to_string(std::get<1>(x)) + " " + std::to_string(std::get<2>(x)) + ")";
  };
  std::function<std::string(int)> f2 = [=] (int x) {return std::to_string(x);};
  return toGraphwizWith(f1, f2);
}


/*
 * Function converting the automaton <string, string> to gff format.
 * @return Gff representation of the automaton
 */
template <>
std::string GeneralizedCoBuchiAutomaton<std::string, std::string>::toGff(){
  std::function<std::string(std::string)> f1 = [&] (std::string x) {return x;};
  std::function<std::string(std::string)> f2 = [&] (std::string x) {return x;};
  return toGffWith(f1, f2);
}

template<>
std::string GeneralizedCoBuchiAutomaton<int, int>::toGff(){
  std::function<std::string(int)> f1 = [&] (int x) {return std::to_string(x);};
  std::function<std::string(int)> f2 = [&] (int x) {return std::to_string(x);};
  return toGffWith(f1, f2);
}


template<>
std::string GeneralizedCoBuchiAutomaton<int,std::string>::toHOA(){
    std::string res;
  size_t alph_size = this->alph.size();
  res += "HOA: v1\n";
  res += "States: " + std::to_string(this->states.size()) + "\n";

  // renumber states to be a continuous sequence
  std::map<int, size_t> state_to_seq;
  size_t state_cnt = 0;
  for (auto st : this->states) {
    state_to_seq.insert({st, state_cnt++});
  }

  // initial states
  for (auto st : this->initials) {
    res += "Start: " + std::to_string(state_to_seq[st]) + "\n";
  }

  res += "acc-name: generalized-co-Buchi " + std::to_string(this->finals.size()) + "\n";
  res += "Acceptance: " + std::to_string(this->finals.size()) + " ";
  for (unsigned i=0; i<this->finals.size(); i++){
      if (i != 0)
        res += "|";
    res += "Fin(" + std::to_string(i) + ")";
  }
  res += "\n";
  res += "properties: trans-labels explicit-labels state-acc\n";
  res += "AP: " + std::to_string(alph_size);

  // renumber symbols
  std::map<string, size_t> symb_to_pos;
  size_t symb_cnt = 0;
  for (auto symb : this->alph) {
    res += " \"" + symb + "\"";
    symb_to_pos.insert({symb, symb_cnt++});
  }

  // transitions
  res += "\n--BODY--\n";
  for (auto st : this->states) {
    size_t seq_st = state_to_seq[st];
    res += "State: " + std::to_string(seq_st);
    bool empty = true;
    bool end = false;
    for (auto it = this->finals.begin(); it != this->finals.end(); it++){
        if (it->second.find(st) != it->second.end()){
            if (empty)
                res += "{";
            empty = false;
            end = true;
            res += std::to_string(it->first) + " ";
        }
    }
    if (end)
        res += "}";
    res += "\n";

    for (auto symb : this->alph) {
      auto it = this->trans.find({st, symb});
      if (it == this->trans.end() || it->second.empty()) continue;

      // construct the string for the symbol first (composed of atomic propositions)
      std::string symb_str;
      symb_str += "[";
      bool first = true;
      for (size_t i = 0; i < alph_size; ++i) {
        if (first) first = false;
        else symb_str += " & ";

        if (symb_to_pos[symb] != i) symb_str += "!";
        symb_str += std::to_string(i);
      }
      symb_str += "]";

      for (auto dst : it->second) {
        res += symb_str + " " + std::to_string(state_to_seq[dst]) + "\n";
      }
    }
  }

  res += "--END--\n";

  return res;
}  

template<typename State, typename Symbol>
GeneralizedCoBuchiAutomaton<int, int> GeneralizedCoBuchiAutomaton<State,Symbol>::renameAutDict(map<Symbol, int>& mpsymbol, int start){
    int stcnt = start;
    std::map<State, int> mpstate;
    std::set<int> rstate;
    Delta<int, int> rtrans;
    std::map<int, std::set<int>> rfin;
    std::set<int> rini;
    set<int> rsym;
    this->invRenameMap = std::vector<State>(this->states.size() + start);

    for(auto st : this->states)
    {
      auto it = mpstate.find(st);
      this->invRenameMap[stcnt] = st;
      if(it == mpstate.end())
      {
        mpstate[st] = stcnt++;
      }
    }
    for(const auto& t : this->alph)
    {
      rsym.insert(mpsymbol[t]);
    }

    rstate = Aux::mapSet(mpstate, this->states);
    rini = Aux::mapSet(mpstate, this->initials);
    rfin = Aux::mapMap(mpstate, this->finals);
    for(auto p : this->trans)
    {
      int val = mpsymbol[p.first.second];
      std::set<int> to = Aux::mapSet(mpstate, p.second);
      rtrans.insert({std::make_pair(mpstate[p.first.first], val), to});
    }

    auto ret = GeneralizedCoBuchiAutomaton<int, int>(rstate, rfin, rini, rtrans, rsym);
    this->renameStateMap = mpstate;
    this->renameSymbolMap = mpsymbol;

    ret.setAPPattern(this->apsPattern);
    return ret;
}

template<>
void GeneralizedCoBuchiAutomaton<int,int>::removeUseless(){
    //TODO
    // remove inherently weak sccs (for all fins) which do not have any non-inherently weak scc successor
}
    
template<>
bool GeneralizedCoBuchiAutomaton<int,int>::isEmpty(){
    //TODO
    // empty <=> there are only inherently weak sccs (for all fins)
}

template<typename State, typename Symbol>
GeneralizedCoBuchiAutomaton<std::tuple<State, int>, Symbol> GeneralizedCoBuchiAutomaton<State, Symbol>::product(GeneralizedCoBuchiAutomaton<int, Symbol> &other){
  typedef tuple<State, int> ProdState;
  
  // product automaton + distribution of fins
  GeneralizedCoBuchiAutomaton<std::tuple<State, int>, Symbol> cartProduct = this->cartProduct(other);

  std::map<int, std::set<ProdState>> nfin;
  std::set<ProdState> emptySet;
  emptySet.clear();
  for (unsigned i=0; i<this->getFinals().size()*other.getFinals().size(); i++)
    nfin.insert({i, emptySet});
  for (auto state : cartProduct.getStates()){
    for (unsigned i=0; i<this->finals.size(); i++){
      if (this->finals[i].find(std::get<0>(state)) != this->finals[i].end()){
        // final in first automaton
        for (auto index = this->finals.size()*i; index < (i+1)*other.getFinals().size()-1; index++)
          nfin[index].insert(state);
      }
    }

    for (unsigned i=0; i<other.getFinals().size(); i++){
      if (other.getFinals()[i].find(std::get<1>(state)) != other.getFinals()[i].end()){
        // final in second automaton
        for (auto index = i; i < this->getFinals().size()*other.getFinals().size(); i += other.getFinals().size())
          nfin[index].insert(state);
      }
    }
  }

  return GeneralizedCoBuchiAutomaton<tuple<State, int>, Symbol>(cartProduct.getStates(), nfin, cartProduct.getInitials(), cartProduct.getTransitions());
}

template<typename State, typename Symbol>
GeneralizedCoBuchiAutomaton<std::tuple<State, int>, Symbol> GeneralizedCoBuchiAutomaton<State, Symbol>::cartProduct(GeneralizedCoBuchiAutomaton<int, Symbol> &other){
  typedef tuple<State, int> ProdState;
  set<ProdState> nstates;
  set<ProdState> nini;
  stack<ProdState> stack;
  map<int,set<State>> fin1 = this->getFinals();
  map<int,set<int>> fin2 = other.getFinals();
  auto tr1 = this->getTransitions();
  auto tr2 = other.getTransitions();
  map<std::pair<ProdState, Symbol>, set<ProdState>> ntr;
  map<int,set<ProdState>> nfin;

  for(const State& st1 : this->getInitials())
  {
    for(const int& st2 : other.getInitials())
    {
      stack.push({st1, st2});
      nstates.insert({st1, st2});
      nini.insert({st1, st2});
    }
  }

  while(stack.size() > 0)
  {
    ProdState act = stack.top();
    stack.pop();

    for(const Symbol& sym : this->getAlphabet())
    {
      set<ProdState> dst;
      for(const State& d1 : tr1[{std::get<0>(act), sym}])
      {
        for(const int& d2 : tr2[{std::get<1>(act), sym}])
        {
          dst.insert({d1,d2});
        }
      }
      for(auto& item : dst)
      {
        if(nstates.find(item) == nstates.end())
        {
          nstates.insert(item);
          stack.push(item);
          for (auto it = this->getFinals().begin(); it != this->getFinals().end(); it++){
              if (it->second.find(std::get<0>(item)) != it->second.end())
                nfin[it->first].insert(item);
          }
          for (auto it = other.getFinals().begin(); it != other.getFinals().end(); it++){
              if (it->second.find(std::get<1>(item)) != it->second.end())
                nfin[(it->first)+this->getFinals().size()].insert(item);
          }
        }
      }
      ntr[{act, sym}] = dst;
    }
  }

  return GeneralizedCoBuchiAutomaton<tuple<State, int>, Symbol>(nstates, nfin, nini, ntr);
}

template<typename State, typename Symbol>
GeneralizedCoBuchiAutomaton<State, Symbol> GeneralizedCoBuchiAutomaton<State, Symbol>::unionGcoBA(GeneralizedCoBuchiAutomaton<State, Symbol> &other){
  set<State> nstates;
  set<State> nini;
  Transitions ntr(this->getTransitions());
  map<int,set<State>> nfin;

  set_union(this->getStates().begin(), this->getStates().end(), other.getStates().begin(),
    other.getStates().end(), std::inserter(nstates, nstates.begin()));
  set_union(this->getInitials().begin(), this->getInitials().end(), other.getInitials().begin(),
    other.getInitials().end(), std::inserter(nini, nini.begin()));
  
  nfin = this->getFinals();
  // add acceptance sets
  for (auto it = other.getFinals().begin(); it != other.getFinals().end(); it++){
    nfin.insert({nfin.size(), it->second});
  }

  // merge transitions
  for (auto it = other.getTransitions().begin(); it != other.getTransitions().end(); it++){
      if (ntr.find(it->first) != ntr.end())
        ntr[it->first].insert(it->second.begin(), it->second.end());
      else
        ntr.insert({it->first, it->second});
  }

  return GeneralizedCoBuchiAutomaton<State, Symbol>(nstates, nfin, nini, ntr, this->getAlphabet());
}

template<typename State, typename Symbol>
GeneralizedCoBuchiAutomaton<State, Symbol> GeneralizedCoBuchiAutomaton<State, Symbol>::reverse(){
  //TODO
}

template class GeneralizedCoBuchiAutomaton<int, int>;
template class GeneralizedCoBuchiAutomaton<std::string, std::string>;
template class GeneralizedCoBuchiAutomaton<int, APSymbol>;