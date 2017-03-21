#include "RollSet.h"

using namespace std;
using namespace glm;

void RollSet::init(vector<Dice*> dice, const int extra) {
   int rollTotal = 0;
   vector<vec2> positions;
   for (int i = 0; i < dice.size(); ++i) {
      float startX = -10 + (rand() / (float)RAND_MAX) * 20;
      float startZ = -10 + (rand() / (float)RAND_MAX) * 20;
      vec3 start = vec3(startX, 12, startZ);

      vec3 end;
      float endX, endZ;
      bool found;
      int tries;
      const float minDist = 3.0f;
      for (tries = 0, found = false; !found && tries < 500; ++tries) {
         endX = -8 + (rand() / (float)RAND_MAX) * 16;
         endZ = -8 + (rand() / (float)RAND_MAX) * 16;
         found = true;
         for (size_t j = 0; found && j < positions.size(); j++)
            found = sqrt(pow(endX - positions[j].x, 2)
             + pow(endZ - positions[j].y, 2)) > minDist;
      }
      if (tries >= 500)
         cout << "max dice placement tries reached... giving up" << endl;
      positions.push_back(vec2(endX, endZ));
      end = vec3(endX, 0, endZ);

      int bounces = 2 + rand() % 4;

      Material *m1 = new Material();
      Material *m2 = new Material();
      Material::generateRandom(*m1, *m2);
      // cout << "--- " << dice[i]->getName() << " ---" << endl;
      // m1->print();
      // m2->print();

      Roll *r = new Roll(dice[i], m1, m2, start, end, bounces,
       vec2(-10, 10), vec2(-10, 10));
      rollTotal += r->getValue();
      rolls.push_back(r);
   }

   if (extra) {
      cout << "adding " << extra << " to the total" << endl;
      rollTotal += extra;
   }

   cout << "total: " << rollTotal << endl;
   for (int i = 0; i < rolls.size(); ++i) {
      rolls[i]->start();
   }
}

using namespace std::regex_constants;
RollSet::RollSet(const string &description, Dice *allDice[], int diceCount) {
   try {
      // make sure the whole description matches
      regex roll_re("(^[1-9]?d(?:4|6|8|100|12|20|10)(?:\\+[1-9]?d(?:4|6|8|100|12|20|10)){0,3}(?:\\+\\d+)?$)", icase);
      smatch roll_match;
      if (!regex_search(description, roll_match, roll_re) || roll_match.size() <= 1) {
         cerr << "Invalid roll description: " << description << endl;
         cerr << "Consult the README for more info..." << endl;
         return;
      } else {
         cout << "description: " << description << endl;
      }

      // iterate through individual terms
      vector<Dice*> roll_dice;
      regex dice_re("[1-9]?d(?:4|6|8|100|12|20|10)", icase);
      sregex_iterator next(description.begin(), description.end(), dice_re);
      sregex_iterator end;
      while (next != end) {
         smatch match = *next;
         cout << " - " << match.str() << endl;

         // determine the quantity
         int d_at = match.str().find('d');
         int quantity = 1;
         if (d_at == 1) {
            quantity = atoi(match.str().substr(0,1).c_str());
         }

         // determine the dice type
         string type_str = match.str().substr(d_at + 1);
         int d_type = atoi(type_str.c_str());
         Dice *dice = NULL;
         Dice *other = NULL;
         switch (d_type) {
         case 4:
            dice = allDice[1];
            break;
         case 6:
            dice = allDice[0];
            break;
         case 8:
            dice = allDice[2];
            break;
         case 10:
            dice = allDice[3];
            break;
         case 12:
            dice = allDice[4];
            break;
         case 20:
            dice = allDice[5];
            break;
         case 100:
            dice = allDice[3];
            other = allDice[6];
            break;
         default:
            cerr << "unknown dice type: " << d_type << endl;
            exit(1);
         }

         // add the dice
         for (size_t i = 0; i < quantity; i++) {
            roll_dice.push_back(dice);
            if (other != NULL)
               roll_dice.push_back(other);
         }

         next++;
      }

      // add the extra to the total
      int extra = 0;
      regex extra_re("(^.*\\+\\d+$)", icase);
      smatch extra_match;
      if (regex_search(description, extra_match, extra_re) && extra_match.size() > 1) {
         regex num_re("\\d+$");
         sregex_iterator extra_finder(description.begin(), description.end(), num_re);
         smatch extra_match = *extra_finder;
         extra = atoi(extra_match.str().c_str());
      }

      // initialize!
      init(roll_dice, extra);

   } catch (regex_error &e) {
      cerr << "bad regex syntax" << endl;
   }
}

RollSet::RollSet(Dice *allDice[], int diceCount) {
   vector<Dice*> dice;
   dice.push_back(allDice[3]);
   dice.push_back(allDice[6]);
   init(dice, 0);
}

RollSet::~RollSet() {
   for (int i = 0; i < rolls.size(); ++i) {
      delete rolls[i];
   }
}

void RollSet::draw(shared_ptr<Program> prog, shared_ptr<MatrixStack> M) {
   for (int i = 0; i < rolls.size(); ++i) {
      rolls[i]->draw(prog, M);
   }
}
