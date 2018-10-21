#include "../include/world.hpp"

namespace cleaner{
  state*const world::getStartState() const{
    return this->states[0];
  }

  int world::getNumStates() const{
    return this->states.size();
  }

  state*const world::getState(int i) const{
    return this->getNumStates() > i ? this->states[ i ] : NULL;
  }

  // check the dynamics model
  bool world::sound(double tolerance) const{
    for(int a=0; a<action::END; ++a) {
      for(auto s : this->states){
        double prob = 0.0;

        for(auto ss : this->states){
          prob += this->probability(s,static_cast<action>(a), ss);
        }

        if( std::abs(1-prob) > tolerance ){  return false;  }
      }
    }

    return true;
  }

  void world::populate(){
    std::vector<bool> grid = this->init(width * height);

    int battery, pose, base = 0;
    do{
      for( pose=0; pose<width*height; ++pose )
        for( battery=0; battery<cbattery; ++battery ){
          states.push_back( new state(grid, base==pose, battery, pose) );
        }
    } while( (grid = this->next(width * height)).size() != 0 );
  }

  std::vector<bool> world::init(size n){
    this->cbegin = new bool[ n ];
    std::fill_n(this->cbegin, n, false);
    this->cend = this->cbegin + (n-1);
    this->current = this->cend;

    std::vector<bool> grid(n);
    std::copy(this->cbegin, this->cend+1, grid.begin());
    return grid;
  }

  std::vector<bool> world::next(size n){
    this->current = this->cend;

    //<! easy case, increase rightmost element
    if( !(*this->current) ) {
      *this->current = true;
      std::vector<bool> grid(n);
      std::copy(this->cbegin, this->cend+1, grid.begin());
      return grid;
    }

    //<! find rightmost element to increase && reset right-hand elements
    do{
      *this->current = false; this->current--;
    }while( this->current >= this->cbegin && *this->current );

    //<! terminate if all elements attained their limits
    if( this->current < this->cbegin ){
      this->current = this->cend;
      return std::vector<bool>();
    }

    //<! else, increase the current value, && return the output.
    *this->current = true;
    std::vector<bool> grid(n);
    std::copy(this->cbegin, this->cend+1, grid.begin());
    return grid;

    return grid;
  }

  world::world(size width, size height, size cbattery) : width(width), height(height), cbattery(cbattery){
    this->populate();
  }

  world::~world(){
    for (std::vector< state* >::iterator it = this->states.begin() ; it != this->states.end(); ++it){
     delete (*it);
    } this->states.clear();

    if( cbegin != NULL ){
      delete[] cbegin;
      cbegin = NULL;
    }
  }

  double world::reward(state* const s, action a)  const{
    std::vector<bool> grid = s->getGrid();
    bool  status = s->getBase();

    int i;
    for(i=0; i<this->width * this->height && status; ++i){
      status &= grid[i];
    }

    // final state
    if( status ) return 0;

    // penalty for taking action wait
    else if( a == action::WAIT || a == action::CHARGE ) return -0.1;

    // default penalty
    return -1;
  }

  bool world::compare(std::vector<bool>const& v1, std::vector<bool>const& v2) const{
    return this->compare(v1, v2, -1);
  }

  bool world::compare(std::vector<bool>const& v1, std::vector<bool>const& v2, int pose) const{
    bool ok = true;
    for(int i=0; i<v1.size() && ok; ++i) if(pose != i) ok &= v1[i] == v2[i];
    return ok;
  }

  double world::probability(state* const  s, action a, state* const  ss)  const{
    std::vector<bool> g, gg;
    int max, x,y, xx, yy, x_, y_, b, bb, b_;
    max = this->height >= this->width ? this->height : this->width;

    x = s->getPose() / max;
    y = s->getPose() % max;
    b = s->getBattery();
    g = s->getGrid();

    xx = ss->getPose() / max;
    yy = ss->getPose() % max;
    bb = ss->getBattery();
    gg = ss->getGrid();

    switch(a){
      case action::LEFT    :
      x_ = x - 1; y_ = y;
      if( b==0  && ss==s ){
        return 1.0;
      }
      else if( x_ < 0 && x == xx && y == yy && this->compare(g,gg) && bb==b-1){
        return 1.0;
      }
      else if( x_ >= 0 && x == xx && y == yy && this->compare(g,gg) && bb==b-1 ){
        return this->proba;
      }
      else if( x_ == xx && y_ == yy && this->compare(g,gg) && bb==b-1){
        return 1-this->proba;
      }
      break;

      case action::RIGHT   :
      x_ = x + 1; y_ = y;
      if( b == 0  && ss==s){
        return 1.0;
      }
      else if( x_ >= this->width  && x == xx && y == yy && this->compare(g,gg) && bb==b-1){
        return 1.0;
      }
      else if( x_ < this->width && x == xx && y == yy && this->compare(g,gg) && bb==b-1){
        return this->proba;
      }
      else if( x_ == xx && y_ == yy && this->compare(g,gg) && bb==b-1){
        return 1-proba;
      }
      break;

      case action::UP      :
      x_ = x; y_ = y + 1;
      if( b == 0  && ss==s){
        return 1.0;
      }
      else if( y_ >= this->height  && x == xx && y == yy && this->compare(g,gg)  && bb==b-1){
        return 1.0;
      }
      else if( y_ < this->height && x == xx && y == yy && this->compare(g,gg) && bb==b-1){
        return this->proba;
      }
      else if( x_ == xx && y_ == yy && this->compare(g,gg) && bb==b-1){
        return 1-proba;
      }
      break;

      case action::DOWN    :
      x_ = x; y_ = y - 1;
      if( b == 0  && ss==s){
        return 1.0;
      }
      else if( y_ < 0  && x == xx && y == yy && this->compare(g,gg) && bb==b-1){
        return 1.0;
      }
      else if( y_ >= 0 && x == xx && y == yy && this->compare(g,gg)  && bb==b-1){
        return this->proba;
      }
      else if( x_ == xx && y_ == yy && this->compare(g,gg)  && bb==b-1){
        return 1-proba;
      }
      break;

      case action::CHARGE  :
      b_ = b+1;
      if( b_ >= this->cbattery  && ss == s ){
        return 1.0;
      }
      else if( b_ <= this->cbattery && ss == s ){
        return this->proba;
      }
      else if( b_ <= this->cbattery && b_ == bb && x == xx && y == yy && this->compare(g,gg) ){
        return 1-proba;
      }
      break;

      case action::WAIT    :
      if( s == ss ){
        return 1.0;
      }
      break;

      case action::CLEAN   :
      if( b < 2  && ss==s){
        return 1.0;
      }
      else if( (x == xx) && (y == yy) && this->compare(g,gg) && g[s->getPose()] && (bb==b-2) ){
        return 1.0;
      }
      else if( x == xx && y == yy && this->compare(g,gg) && g[s->getPose()] == false && bb==b-2){
        return this->proba;
      }
      else if( x == xx && y == yy && this->compare(g,gg,s->getPose()) && g[s->getPose()] == false && gg[ss->getPose()] == true && bb==b-2){
        return 1-this->proba;
      }
      break;

      default             :
      return 0.0;
    }

    return 0.0;
  }

  void world::execute(int s, action a, int& ss, double& r){
    double prob = 0, rd = rand() / ((double) RAND_MAX);
    for(int i=0; i<this->getNumStates(); ++i){
      prob += this->probability(this->states[s],a,this->states[i]);
      if( rd <= prob ) {
        ss = i;
        r = this->reward(this->states[s],a);
        return;
      }
    }
  }

  std::vector<state*>const& world::getStates() const{
    return this->states;
  }

}