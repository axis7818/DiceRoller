#ifndef _ROLL_SET_H
#define _ROLL_SET_H

#include <iostream>
#include <regex>
#include <vector>
#include "Material.h"
#include "Dice.h"
#include "Roll.h"
#include "Program.h"
#include "MatrixStack.h"

class RollSet {
   std::vector<Roll*> rolls;
   int total;

   void init(std::vector<Dice*> dice, const int extra);

public:
   RollSet(const std::string &description, Dice *allDice[], int diceCount);
   RollSet(Dice *allDice[], int diceCount);
   virtual ~RollSet();

   void draw(std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack> M);
   int getTotal() const { return total; }
};

#endif /* end of include guard: _ROLL_SET_H */
