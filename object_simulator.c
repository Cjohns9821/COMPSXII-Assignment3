#include <stdio.h>
#include <string.h>

/* Forward declaration */
typedef struct Character Character;

/* ============================================================
   PART 1 — BASE CHARACTER STRUCT
   ============================================================ */

struct Character {
    char name[50];
    int health;
    int level;

    void (*attack)(Character *self);
    void (*take_damage)(Character *self, int dmg);
};

/* ============================================================
   PART 2 — BASE CHARACTER METHODS
   ============================================================ */

void character_attack(Character *self) {
    printf("%s performs a basic attack!\n", self->name);
}

void character_take_damage(Character *self, int dmg) {
    self->health -= dmg;
    if (self->health < 0) self->health = 0;

    printf("%s takes %d damage! Health now %d\n",
           self->name, dmg, self->health);
}

Character create_character(const char *name, int health, int level) {
    Character c;

    strncpy(c.name, name, sizeof(c.name));
    c.name[sizeof(c.name) - 1] = '\0';

    c.health = health;
    c.level = level;

    c.attack = character_attack;
    c.take_damage = character_take_damage;

    return c;
}

/* ============================================================
   PART 3 — DERIVED TYPES (Warrior, Mage)
   ============================================================ */

typedef struct Warrior {
    Character base;   // inheritance via composition
    int strength;
} Warrior;

typedef struct Mage {
    Character base;
    int mana;
} Mage;

/* ============================================================
   PART 4 — POLYMORPHIC METHODS
   ============================================================ */

void warrior_attack(Character *self) {
    Warrior *w = (Warrior *)self;  // safe cast because base is first member
    printf("%s swings a sword for heavy damage! (Strength: %d)\n",
           w->base.name, w->strength);
}

void mage_attack(Character *self) {
    Mage *m = (Mage *)self;
    printf("%s casts a fireball! (Mana: %d)\n",
           m->base.name, m->mana);
}

Warrior create_warrior(const char *name, int health, int level, int strength) {
    Warrior w;
    w.base = create_character(name, health, level);
    w.strength = strength;

    w.base.attack = warrior_attack;          // override
    w.base.take_damage = character_take_damage;

    return w;
}

Mage create_mage(const char *name, int health, int level, int mana) {
    Mage m;
    m.base = create_character(name, health, level);
    m.mana = mana;

    m.base.attack = mage_attack;             // override
    m.base.take_damage = character_take_damage;

    return m;
}

/* ============================================================
   PART 5 — MAIN TESTING
   ============================================================ */

int main() {

    Character hero = create_character("Basic Hero", 80, 1);
    Warrior thor = create_warrior("Thorin", 41, 5, 10);
    Mage cloud = create_mage("Cloud", 23, 4, 30);

    printf("=== Individual Attacks ===\n");
    hero.attack(&hero);
    thor.base.attack(&thor.base);
    cloud.base.attack(&cloud.base);

    printf("\n=== Polymorphism Demo ===\n");
    Character *party[3] = { &hero, &thor.base, &cloud.base };

    for (int i = 0; i < 3; i++) {
        party[i]->attack(party[i]);  // same call, different behavior
    }

    printf("\n=== Damage Demo ===\n");
    party[1]->take_damage(party[1], 15);
    party[2]->take_damage(party[2], 22);

    return 0;
}
