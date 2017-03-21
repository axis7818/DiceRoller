# Dice Roller

Cameron Taylor

W2017 CPE 471

---

## How To Use

```
Usage: ./dice RESOURCE_DIR ROLL_DESCRIPTION

   ./dice           : the program name
   RESOURCE_DIR     : the directory that holds the project's resources
   ROLL_DESCRIPTION : which/how many dice are rolled
```

### Roll Description

The roll syntax looks like this:

- `2d4+d6` rolls 2 four-sided dice and 1 six-sided dice.
- `3d8+d20` rolls 3 eight-sided dice and 1 twenty-sided dice.
- `3d12+1d4+1d6` rolls 3 twelve-sided dice, 1 four-sided dice, and 1 six-sided dice.
- `5d12+18` rolls 5 twelve-sided dice and adds 18 to the total.

Some rules:

- there must be at least one dice term, and at most 4. The last "extra value" term does not count as a term.
- there can only be one "extra value" term applied to the very end of the roll description.
- the coefficient of any given term can be anything from 1 to 9. If omitted, 1 is assumed.
- the options for dice types are `d4`, `d6`, `d8`, `d10`, `d12`, `d20`, and `d100`. The `d100` is actually two ten-sided dice. One has numbers 0-9, and the other has numbers 00-90 in increments of 10. So, a `d100` can result in a 70 and a 4. 74.

If any of the syntax rules are violated, a warning will be displayed on the console and no dice will roll. If the ROLL_DESCRIPTION is omitted, the default description of `d100` will roll.

---

## Important Files

### main.cpp

This is driver for the program. It takes care of OpenGL initialization, the window/key callbacks,
and pushes the render loop. Main also manages the reference to the RollSet instance.

### RollSet.h/cpp

The RollSet class manages a vector of Roll instances and makes the decisions of what kind of
dice will be rolled. This class also tracks the total value of the roll.

### Roll.h/cpp

A Roll represents the roll of a single dice. It decides (randomly) what face the dice will land
on and manages the dice's position and rotation as it executes the bounces. The roll class ensures
that a dice will land on its correct face at the end of all of the bounces.

### Bounce.h/cpp

A bounce describes the translational movement of a dice as it is rolled. There is a starting
point, and ending point, and a mid point. For now, the bounces are two linear motions that
the dice follows, but could be extended to follow a parabolic path.

### Dice.h/cpp

A Dice manages a shape, texture, and a list of faces associated with that dice. This is the class that represents the model that is drawn in the world.

### Face.h/cpp

A face has a value and a target theta and phi rotation value that is calculated based off of a
normal vector to that face. This allows a Roll to target a specific face without taking into
account the forces of physics. Face normals are calculated from a region of a dice's
texture coordinates. The dice's shape is searched for points whose texture coordinates lie
in this region, and when one is found, its respective normal vector is used as that
face's normal.

### Material.h/cpp

Each Roll has two materials that is associated with it. These materials are randomly
generated, but generated with a purpose. and in such a way that complete flukes of randomness are
avoided. different color profiles are defined and chosen in ways that contrast the dice bodies
with their numbers, and the materials are chosen to look realistic for dice.

### MyProg.h/cpp

A wrapper to the Program class that prevents retyping common uniform and attribute names.

### Skybox.h/cpp

A class that manages the skybox.

### ct_frag.glsl

This fragment shader uses Cook-Torrance shading with the following parameters:

   - Ambient Color (r, g, b)
   - opacity (0 -> 1, though this is not fully supported as it simply controls the alpha value)
   - roughness (0 is smooth, 1 is rough)
   - F0 or the fresnel reflectance. This is the proportion of light that is reflected at normal incidence.
   - K the fraction of diffuse reflection. Specular reflection is determined to be 1 - K.
   - clipMode (0: nothing, 1: clips the black fragments, 2: clips the white fragments. These are used to apply materials to the bodies/numbers independently)

Sources for this shader:

- http://www.codinglabs.net/article_physically_based_rendering_cook_torrance.aspx
- http://ruh.li/GraphicsCookTorrance.html
- https://renderman.pixar.com/view/cook-torrance-shader
