// TxtAdvent.cpp : A programming challenge to make a text-based adventure game while not using object-oriented programming
//

#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h> 
#include <iostream>

using namespace std;

/*
TODO

Apply player's attack algorithm to enemies (tried, worked with goblin but barbarian kept dealing the same damage)
*/

struct item {
	string name;
	string desc;
};

struct tile {
	tile* N;
	tile* S;
	tile* E;
	tile* W;
	string desc; //send when player gives a look command
	bool doneEvent; //determine if a certain event is done
	string event; //identify the event
	string eObj; //object to use to complete the event
	int number; //identify the tile
	vector<item> loot;
};

struct weapon{
	string name;
	int dmg;
};

struct armor{
	string name;
	int def;
};

struct player {
	weapon w;
	armor a;
	vector<item> inv;
	int health;
};

struct enemy {
	string name;
	int atck; //multiplier for when atck() is called.
	vector<item> loot;
	int health;
};

int crit(){
	int crit = (rand() % 10) + 1;
	if(crit == 1 || crit == 2 || crit == 8)return 2;
	return 1;
}

int atck(){
	return(rand() % 10) + 1;
}

int defend(){
	int ret = (rand() % 10) + 1;
	return ret * (rand() % 10) + 1;
}

item key;
weapon fists;
weapon club;
weapon sword;
weapon axe;
weapon cythe;
enemy g;
enemy b;
enemy o;
enemy d;
player p;
void setup(){
	p.health = 100;
	key.name = "key";
	key.desc = "A shiny gold key.";
	fists.name = "fists";
	fists.dmg = 1;
	club.name = "Club";
	club.dmg = 2;
	sword.name = "Sword";
	sword.dmg = 3;
	axe.name = "Axe";
	axe.dmg = 4;
	cythe.name = "Cythe";
	cythe.dmg = 5;
	g.name = "Goblin";
	g.atck = 1;
	g.health = 20;
	b.name = "Barbarian";
	b.atck = 2;
	b.health = 100;
	o.name = "Orc";
	o.atck = 3;
	o.health = 200;
	d.name = "Demon";
	d.atck = 5;
	d.health = 500;
	p.w = fists;
}

tile* ct = new tile; // current tile
tile* world[32];
void setupTiles(){
	for(int i = 0; i < 32; i++){ //world init
		world[i] = new tile;
		world[i]->doneEvent = false;
		world[i]->number = i;
	}
	
	for(int i = 0; i < 28; i+=7){ //east init
		for(int j = i; j < i + 6; j++){
			world[j]->E = world[j+1];
		}
	}
	for(int i = 0; i < 28; i+=7){ //west init
		for(int j = i+6; j > i; j--){
			world[j]->W = world[j-1];
		}
	}
	for(int i = 0; i < 7; i++){ //south init
		for(int j = i; j < 28; j+=7){
			world[j]->S = world[j+7];
		}
	}
	for(int i = 21; i < 28; i++){ //north init
		for(int j = i; j > 6; j-=7){
			world[j]->N = world[j-7];
		}
	}
	
	char ten = '0';
	char one = '0';
	for(int i = 0; i < 28; i++){
		if(i != 24){
			world[i]->desc = string() + ten + one; //change tile desc to tile number
		}
		if (one == '9'){
			one = '0';
			ten++;
		}else one++;
	}
	
	
	world[28]->E = world[29];
	world[29]->W = world[28];
	world[29]->N = world[24];
	world[29]->E = world[30];
	world[30]->W = world[29];
	world[29]->S = world[31];
	world[31]->N = world[29];
	world[24]->S = world[29];
	
	world[31]->desc = "You see walls of old gray bricks surrounding you, safe for a locked cell gate just in front of you to the north. You look at your feet to see a small gold key.";
	world[31]->event = "gateN";
	world[31]->eObj = "key";
	world[31]->loot.push_back(key);
	world[29]->desc = "You see a grouling rabid goblin to the north. He seems ready to attack you. To your east and west you see two paths leading further into the dungeon, and behind the goblin you see the warm glow of sunlight."; //change when goblin is kill
	world[29]->event = "goblin";
	world[30]->desc = "You reach a dead end with nothing but an unopened small chest in front of you.";
	world[30]->event = "chest";
	world[28]->desc = "You see a small chest in front of you. Surrounding you are gray brick walls, safe for the entryway you came from.";
	world[28]->event = "chest";
	world[24]->desc = "You surrounded by a forest. Behind you is the dungeon you found yourself in. In front of you is an oldman trying to get your attention."; //change when talk oldman
	world[24]->event = "oldman";
	world[17]->event = "barbarian";
	world[10]->event = "orc";
	world[3]->event = "demon";
	ct = world[31];
	/*cout << "Debug mode: starting point changed." << endl;
	ct = world[29];*/
}

void clear(tile* t){
	if(t->N)clear(t->N);
	if(t->W)clear(t->W);
	while(t->E){
		t->doneEvent = NULL;
		t = t->E;
		if(t->W->N) t->W->N = NULL;
		t->W->E = NULL;
		if(t->W->S) t->W->S = NULL;
		t->W = NULL;
	}
}

void clear(){
	tile* t = world[31];
	t->doneEvent = NULL; //del 31 event
	t = t->N; // t = 29
	t->S->N = NULL;
	t->S = NULL; // del 29 S
	t->doneEvent = NULL; //del 29 event
	t = t->E; // t = 30
	t->doneEvent = NULL; //del 30 event
	t = t->W; // t = 29 
	t->E->W = NULL; //del 30 W
	t->E = NULL;// del e5
	t = t->W; // t = c5
	t->doneEvent = NULL; //c5
	t = t->E; // t = d5 
	t->W->E = NULL;
	t->W = NULL; //del c5
	t = t->N; // t = d4
	clear(t);
	ct = NULL;
	for(int i = 0; i < 32; i++){
		world[i]->N = NULL;
		world[i]->E = NULL;
		world[i]->S = NULL;
		world[i]->W = NULL;
		world[i]->doneEvent = NULL;
		world[i] = NULL;
	}
}

void move(tile* t){
	if(ct->event == "goblin" && ct->doneEvent == false){
		cout << "The goblin prevents you from going anywhere." << endl;
		return;
	}else if(ct->event == "barbarian" && ct->doneEvent == false){
		cout << "The barbarian prevents you from going anywhere." << endl;
		return;
	}else if(ct->event == "orc" && ct->doneEvent == false){
		cout << "The orc prevents you from going anywhere." << endl;
		return;
	}else if(ct->event == "demon" && ct->doneEvent == false){
		cout << "The demon prevents you from going anywhere." << endl;
		return;
	}else if(ct->event == "gateN" && ct->doneEvent == false && (t->number == ct->number-7 || ct->number == 31)){
		cout << "The gate prevents you from going in that direction." << endl;
		return;
	}else if(ct->doneEvent == false){
		cout << "You feel that you missed something but go along anyway." << endl;
	}
	ct = t;
}

void changeEvent(tile* t){
	t->doneEvent = true;
	if(t->number == 31){
		cout << "The gate unlocks with a click and opens by itself. Unfortunately, the key breaks as you take it out." << endl;
		t->desc = "You see walls of old gray bricks surrounding you, safe for an open cell gate in front of you.";
	}else if(t->number == 29){
		t->desc = "You see a dead goblin in front of you. To your left and right you see two paths leading further into the dungeon, and behind the dead goblin you see the warm glow of sunlight.";
		t->event = "dgoblin";
	}else if(t->number == 30){
		/*cout << "You open the chest to find a club! You replace your " << p.w.name << " for it as your weapon." << endl;
		p.w = club;*/
		t->desc = "You reach a dead end with nothing but an opened small chest in front of you.";
	}else if(t->number == 24){
		cout << "PLACEHOLDER oldman rambles about plot or whatever demon is final boss" << endl;
		t->desc = "You surrounded by a forest. Behind you is the dungeon you found yourself in.";
	}else if(t->number == 28){
		cout << "You open the chest to find a club! You replace your " << p.w.name << " for it as your weapon." << endl;
		p.w = club;
		t->desc = "You reach a dead end with nothing but an opened small chest in front of you.";
	}else if(t->number == 17){
		t->event = "dbarbarian";
	}else if(t->number == 10){
		t->event = "dorc";
	}else if(t->number == 3){
		t->event = "ddemon";
	}
}

void get(string object){
	for(int i = 0; i < ct->loot.size(); i++){
		if(object == ct->loot[i].name){
			cout << "You picked up the " << object << " and put it in your invetory." << endl;
			p.inv.push_back(ct->loot[i]);
			ct->loot.erase(ct->loot.begin() + i);
			return;
		}
	}
	for(int i = 0; i < p.inv.size(); i++){
		if(object == p.inv[i].name){
			cout << "You already have the " << object << "." << endl;
			return;
		}
	}
	cout << "There is no " << object << " to get." << endl;
}

void look(string object){
	for(int i = 0; i < ct->loot.size(); i++){
		if(object == ct->loot[i].name){
			cout << ct->loot[i].desc << endl;
			return;
		}
	}
	for(int i = 0; i < p.inv.size(); i++){
		if(object == p.inv[i].name){
			cout << p.inv[i].desc << endl;
			return;
		}
	}
	if(object == "goblin"){
		if(ct->event == "goblin"){
			cout << "A short green beast. Don't let it's size fool you, it's known for ending young adventurers' careers early." << endl;
			return;
		}else if(ct->event == "dgoblin"){
			cout << "The goblin lies dead in a pool of its own blood." << endl;
			return;
		}
	}else if(object == "barbarian"){
		if(ct->event == "barbarian"){
			cout << "A fierce, burly man. Relavtively unassuming when alone, but in groups they are known to burn villages to the ground." << endl;
			return;
		}else if(ct->event == "dbarbarian"){
			cout << "The barbarian lies dead." << endl;
			return;
		}
	}else if(object == "orc"){
		if(ct->event == "orc"){
			cout << "A large, burly green beast. They are known for single-handedly killing entire groups of adventurers." << endl;
			return;
		}else if(ct->event == "dorc"){
			cout << "The orc lies dead. A cause for celebration for most people." << endl;
			return;
		}
	}else if(object == "demon"){
		if(ct->event == "demon"){
			cout << "A foul red beast from hell. Its claws are sharp enough to cut through lesser weapons, its teeth are large enough to pierce steel, and its smirk shows that it knows its power." << endl;
			return;
		}
	}
	cout << "There is no " << object << " to look at." << endl;
}

void use(string object, string thing){
	string eThing = ct->event;
	for(int i = 0; i < p.inv.size(); i++){
		if(object == p.inv[i].name){
			if(thing == "gate"){
				eThing.erase(eThing.end() - 1);
			}
			if(thing == eThing && object == ct->eObj){
				changeEvent(ct);
				p.inv.erase(p.inv.begin() - (i-1));
			}else cout << "Nothing happens." << endl;
			return;
		}
	}
	if(object[0] == 'a' || object[0] == 'e' || object[0] == 'i' || object[0] == 'o' || object[0] == 'u'){
		cout << "You don't have an " << object << "to use." << endl;
	}else{
		cout << "You don't have a " << object << " to use." << endl;
	}
}

void talk(string thing){
	string eThing = ct->event;
	if(thing == "gate"){
		eThing.erase(eThing.end() - 1);
	}
	if(thing == eThing){
		if(thing == "oldman"){
			changeEvent(ct);
		}else if (thing == "goblin" || thing == "barbarian" || thing == "orc" || thing == "demon"){
			cout << "The " << thing << "seems to be too busy killing you to talk to you." << endl;
		}else cout << "The " << thing << " doesn't seem much for conversation." << endl;
	}else if((ct->number == 31 || ct->number == 29 || ct->number == 28 || ct->number == 30) && (thing == "wall" || thing == "brick" || thing == "stone" || thing == "floor")){ //things in dungeon
		cout << "The " << thing << " doesn't seem much for conversation." << endl;
	}else if((ct->number == 24) && (thing == "grass" || thing == "dirt" || thing == "stone" || thing == "ground" || thing == "tree" || thing == "stick" || thing == "rock")){ //things in forest
		cout << "The " << thing << " doesn't seem much for conversation." << endl;
	}else cout << "There is no " << thing << " to talk to." << endl;
}

void attack(string thing){
	string eThing = ct->event;
	if(thing == "gate"){
		eThing.erase(eThing.end() - 1);
	}
	if(thing == eThing){
		if(thing == "oldman"){
			cout << "Now why would you do that?" << endl;
		}else if (thing == "goblin" || thing == "barbarian" || thing == "orc" || thing == "demon"){
			int a;
			int c = crit();
			int def;
			while (a >= 10){
				a = atck();
				def = defend();
				a *= def;
				if (def < 25){
					a /= 5;
				}else if (def < 50){
					a /= 15;
				}else a /= def;
				if (a == 0 || a == 1 || a == 2){
					a = atck();
					def = defend();
					a *= def;
					if (def < 25){
						a /= 25;
					}else if (def < 50){
						a /= 35;
					}else a /= 60;
					if (a == 0){
						a += rand() % 3;
					}
				}
			}
			if(a * c > 10) a*=c;
			a *= p.w.dmg; //factor weapon
			if(a == 0){
				cout << "You try to attack the " << thing << " but miss." << endl;
			}else if(c == 2){
				cout << "Critical hit!" << endl;
				cout << "You deal " << a << " damage to the " << thing << "." << endl;
			}else cout << "You deal " << a << " damage to the " << thing << "." << endl;
			if (thing == "goblin"){
				g.health -= a;
				if(g.health <= 0){
					cout << "The goblin falls to the floor with a shrill death rattle." << endl;
					g.health = 20;
					changeEvent(ct);
				}
			}else if (thing == "barbarian"){
				b.health -= a;
				if(b.health <= 0){
					cout << "The barbarian tries to retaliate but falls to the ground, dead." << endl;
					b.health = 100;
					changeEvent(ct);
				}
			}else if (thing == "orc"){
				o.health -= a;
				if(o.health <= 0){
					cout << "The orc roars loudly before collapsing to the ground." << endl;
					o.health = 200;
					changeEvent(ct);
				}
			}else if (thing == "demon"){
				d.health -= a;
				if(d.health <= 0){
					cout << "The demon yells in unholy agony before disappearing in a puff of smoke." << endl;
					changeEvent(ct);
				}
			}
		}else cout << "You achieve nothing." << endl;
	}else if((ct->number == 31 || ct->number == 29 || ct->number == 28 || ct->number == 30) && (thing == "wall" || thing == "brick" || thing == "stone" || thing == "floor")){ //things in dungeon
		cout << "You achieve nothing." << endl;
	}else if((ct->number == 24) && (thing == "grass" || thing == "dirt" || thing == "stone" || thing == "ground" || thing == "tree" || thing == "stick" || thing == "rock")){ //things in forest
		cout << "You achieve nothing." << endl;
	}else cout << "There is no " << thing << " to attack." << endl;
}

void open(string thing){
		if(ct->event == "chest" && thing == ct->event && ct->doneEvent == false){
			changeEvent(ct);
		}
}

void enemyAttack(){
	if(ct->event == "goblin"){
		int c = crit();
		int a = atck();
		a *= c; //factor crit
		a *= g.atck; //factor dmg
		int def = defend();
		a *= def; //factor def
		if (def < 25){
			a /= 5;
		}else if (def < 50){
			a /= 15;
		}else a /= 50;
		if (a == 1 || a == 2){
			c = crit();
			a = atck();
			def = defend();
			a *= c;
			a *= def;
			if (def < 25){
				a /= 5;
			}else a /= 50;
		}
		if (a <= 20){
			p.health -= a;
		}else{
			enemyAttack();
			return;
		}
		int disHealth = p.health;
		if (disHealth < 0) disHealth = 0;
		if(c == 0 || a == 0){
			cout << "The goblin tries to attack you but misses." << endl;
		}else if(c == 2){
			cout << "Critical hit!" << endl;
			cout << "The goblin deals " << a << " damage to you. You have " << disHealth << " health remaining." << endl;
		}else cout << "The goblin deals " << a << " damage to you. You have " << disHealth << " health remaining." << endl;
	}else if(ct->event == "barbarian"){
		int c = crit();
		int a = atck();
		a *= c; //factor crit
		a *= b.atck; //factor dmg
		int def = defend();
		a *= def; //factor def
		if (def < 25){
			a /= 5;
		}else if (def < 50){
			a /= 15;
		}else a /= 50;
		if (a == 1 || a == 2){
			c = crit();
			a = atck();
			def = defend();
			a *= c;
			a *= def;
			if (def < 25){
				a /= 5;
			}else a /= 50;
		}
		if (a <= 20){
			p.health -= a;
		}else{
			enemyAttack();
			return;
		}
		int disHealth = p.health;
		if (disHealth < 0) disHealth = 0;
		if(c == 0 || a == 0){
			cout << "The barbarian tries to attack you but misses." << endl;
		}else if(c == 2){
			cout << "Critical hit!" << endl;
			cout << "The barbarian deals " << a << " damage to you. You have " << disHealth << " health remaining." << endl;
		}else cout << "The barbarian deals " << a << " damage to you. You have " << disHealth << " health remaining." << endl;
	}else if(ct->event == "orc"){
		int c = crit();
		int a = atck();
		a *= c; //factor crit
		a *= o.atck; //factor dmg
		int def = defend();
		a *= def; //factor def
		if (def < 25){
			a /= 5;
		}else if (def < 50){
			a /= 15;
		}else a /= 50;
		if (a == 1 || a == 2){
			c = crit();
			a = atck();
			def = defend();
			a *= c;
			a *= def;
			if (def < 25){
				a /= 5;
			}else a /= 50;
		}
		if (a <= 20){
			p.health -= a;
		}else{
			enemyAttack();
			return;
		}
		int disHealth = p.health;
		if (disHealth < 0) disHealth = 0;
		if(c == 0 || a == 0){
			cout << "The orc tries to attack you but misses." << endl;
		}else if(c == 2){
			cout << "Critical hit!" << endl;
			cout << "The orc deals " << a << " damage to you. You have " << disHealth << " health remaining." << endl;
		}else cout << "The orc deals " << a << " damage to you. You have " << disHealth << " health remaining." << endl;
	}else if(ct->event == "demon"){
		int c = crit();
		int a = atck();
		a *= c; //factor crit
		a *= d.atck; //factor dmg
		int def = defend();
		a *= def; //factor def
		if (def < 25){
			a /= 5;
		}else if (def < 50){
			a /= 15;
		}else a /= 50;
		if (a == 1 || a == 2){
			c = crit();
			a = atck();
			def = defend();
			a *= c;
			a *= def;
			if (def < 25){
				a /= 5;
			}else a /= 50;
		}
		if (a <= 20){
			p.health -= a;
		}else{
			enemyAttack();
			return;
		}
		int disHealth = p.health;
		if (disHealth < 0) disHealth = 0;
		if(c == 0 || a == 0){
			cout << "The demon tries to attack you but misses." << endl;
		}else if(c == 2){
			cout << "Critical hit!" << endl;
			cout << "The demon deals " << a << " damage to you. You have " << disHealth << " health remaining." << endl;
		}else cout << "The demon deals " << a << " damage to you. You have " << disHealth << " health remaining." << endl;
	}
}

int main(){
	srand (time(NULL));
	setup();
	setupTiles();
	string input;
	string lastInput;
	cout << ct->desc << endl;
	while(p.health > 0){
		getline(cin, input);
		if(input == "re" || input == "repeat"){
			cout << "Repeating \"" << lastInput << "\"." << endl;
			input = lastInput;
		}else lastInput = input;
		string check3; //get, use
		string check4; //look
		string check6; //attack
		if(sizeof input >= 3){
			for(int i = 0; i <= 3; i++){
				check3.insert(i, 1, input[i]);
			}
		}
		if(sizeof input >= 4){
			for(int i = 0; i <= 4; i++){
				check4.insert(i, 1, input[i]);
			}
		}
		if(sizeof input >= 6){
			for(int i = 0; i <= 6; i++){
				check6.insert(i, 1, input[i]);
			}
		}
		
		if(input == "quit" || input == "q"){
			clear();
			return 0;
		}else if(input == "look"){
			cout << ct->desc << endl;
			enemyAttack();
		}else if(input == "north"|| input == "n"){
			if(!ct->N){
				cout << "You can't go that way." << endl;
				enemyAttack();
			}else move(ct->N);
		}else if(input == "east" || input == "e"){
			if(!ct->E){
				cout << "You can't go that way." << endl;
				enemyAttack();
			}else move(ct->E);
		}else if(input == "south" || input == "s"){
			if(!ct->S){
				cout << "You can't go that way." << endl;
				enemyAttack();
			}else move(ct->S);
		}else if(input == "west" || input == "w"){
			if(!ct->W){
				cout << "You can't go that way." << endl;
				enemyAttack();
			}else move(ct->W);
		}
		else if(check3 == "get "){
			while(input[0] != ' ')input.erase(0,1);
			while(input[0] == ' ')input.erase(0,1);
			get(input);
			enemyAttack();
		}else if(check3 == "use "){
			while(input[0] != ' ')input.erase(0,1);
			while(input[0] == ' ')input.erase(0,1);

			string word1; //should be obj
			string word2; //should be on
			string word3 = input; // should be thing
			int i = 0;
			while(word3[0] != ' ' && word3.size() != 0){
				word1.insert(i, 1, word3[0]);
				word3.erase(0,1);
				i++;
			}
			word3.erase(0,1);
			i = 0;
			while(word3[0] != ' ' && word3.size() != 0){
				word2.insert(i, 1, word3[0]);
				word3.erase(0,1);
				i++;
			}
			word3.erase(0,1);
			if(word2 == "on"){
				use(word1, word3);
				enemyAttack();
			}else cout << "What will you use the " << input << " on?"<< endl;
		}else if(check4 == "look "){
			while(input[0] != ' ')input.erase(0,1);
			while(input[0] == ' ')input.erase(0,1);
			look(input);
			enemyAttack();
		}else if(check4 == "talk "){
			while(input[0] != ' ')input.erase(0,1);
			while(input[0] == ' ')input.erase(0,1);
			talk(input);
			enemyAttack();
		}else if(check4 == "open "){
			while(input[0] != ' ')input.erase(0,1);
			while(input[0] == ' ')input.erase(0,1);
			open(input);
			enemyAttack();
		}else if(check6 == "attack "){
			while(input[0] != ' ')input.erase(0,1);
			while(input[0] == ' ')input.erase(0,1);
			attack(input);
			enemyAttack();
		}
		else if(input == "help"){
			cout << "\"look [thing]\": Look at something either in your surroundings or in your invetory.\n\"look\": Look at your surroundings.\n\"get [thing]\": Get something in your surroundings.\n\"talk [thing]\": Talk to something in your surroundings.\n\"[Any cardinal direction or its first letter]\": Go in that direction.\n\"use [thing]\": Use an item in your invetory.\n\"attack [thing]\": Attack something in your surroundings.\n\"open chest\": Opens a chest if there is one in the area.\n\"quit\" or \"q\": Exit the program.\n\"repeat\" or \"re\": Repeat the last command." << endl;
		}
		else{
			cout << "I don't know how to \"" << input << "\"."<< endl;
		}
	}
	cout << "You grow weak and your vision fades as you fall to the ground.\nGame Over" << endl;
	clear();
	return 0;
}