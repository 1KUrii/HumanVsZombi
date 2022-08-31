#include <iostream>
#include <string>
#include <cstdlib> 
#include <ctime> 

int getRandomNumber(int min, int max){
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

class Creature{
protected:
    std::string m_name;
    char m_symbol;
    int m_HP;
    int m_damage;
    int m_gold;
public:
    Creature(std::string name, char symbol, int hp, int damage, int gold)
        : m_name(name), m_symbol(symbol), m_HP(hp), m_damage(damage), m_gold(gold) {}

    const std::string& getName() const { return m_name; }
    char getSymbol() const { return m_symbol; }
    int getHealth() const { return m_HP; }
    int getDamage() const { return m_damage; }
    int getGold() const { return m_gold; }

    void reduceHealth(int value){
        m_HP -= value;
    }

    bool isDead(){
        return (m_HP <= 0);
    }

    void addGold(int gold){
        m_gold += gold;
    }

};
 

class Player : public Creature{
protected:
    int m_level = 1;
public:
    Player(std::string name)
        : Creature(name, '@', 10, 1, 0) {}

    void levelUp(){
        ++m_level;
        ++m_damage;
    }
    
    void Hill(){
        ++m_HP;
    }

    int getLevel(){ return m_level; }

    bool hasWon() { return m_level >= 20;}
};

class Monster : public Creature{
public:
    enum Type{
        DRAGON, 
        ORC, 
        SLIME,
        MAX_TYPES
    };

    struct MonsterData {
        std::string name;
        char symbol;
        int HP;
        int damage;
        int gold;
    };

    Monster(Type type)
        : Creature(monsterData[type].name, monsterData[type].symbol, monsterData[type].HP, monsterData[type].damage, monsterData[type].gold){}


    static MonsterData monsterData[];

    static Monster getRandomMonster(){
		int num = getRandomNumber(0, MAX_TYPES - 1);
		return Monster(static_cast<Type>(num));
	}

};

Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES]{
	{ "dragon", 'D', 20, 4, 100 },
	{ "orc", 'o', 4, 2, 25 },
	{ "slime", 's', 1, 1, 10 }
};
void attackMonster(Monster &m, Player &p);
void attackPlayer(Monster &m, Player &p);
void hpBoster(Player &p);
bool fightMonster(Monster &m, Player &p){
    while (true){
        char temp;
        while(true){
            std::cout << "\n(R)un or (F)ight: "; std::cin>> temp;
            
            if (temp == 'r' || temp == 'R' || temp == 'f' || temp == 'F'){
                std::cin.ignore(32767,'\n');
                break;
            }
            else{
                std::cin.clear();
                std::cin.ignore(32767,'\n');
                std::cout << "Oops, that input is invalid. Please try again";
            }
        }
        
        if (temp == 'r' || temp == 'R'){
            if (getRandomNumber(0, 1)){
                std::cout << "-> You successfully fled\n";
                return true;
            }
            else{
                attackMonster(m, p);
            }
        }
        else{
            attackPlayer(m, p);
            if (!m.isDead()){
                attackMonster(m, p);
            }
        }

        if (p.isDead()){
            std::cout << "\n========================================\n"
            <<"You died at level " << p.getLevel() << " and with " << p.getGold() << " gold\n";
            std::cout << "Too bad you can't take it with you!"
            << "\n========================================\n";
            return false;
        }
        if(m.isDead()){
            std::cout << "-> You killed the " << m.getName() << "\n";
            p.levelUp();
            std::cout << "-> You are now level " << p.getLevel() << "\n";
            p.addGold(m.getGold());
            std::cout << "-> You found " << m.getGold() << " gold\n";
            hpBoster(p);

            std::cout << "========================================\n" 
            << "HP: " << p.getHealth()
            << " Damage: " << p.getDamage() 
            << " Gold: " << p.getGold() 
            << " level: " << p.getLevel() 
            << "\n========================================\n"; 
            return true; 
        }
    }
}

void attackMonster(Monster &m, Player &p){
    p.reduceHealth(m.getDamage());
    std::cout << "The " << m.getName() << " hit you for " << m.getDamage() << " damage\n";
}

void attackPlayer(Monster &m, Player &p){
    m.reduceHealth(p.getDamage());
    std::cout << "You hit the " << m.getName() << " for " << p.getDamage() << " damage\n";
}
void hpBoster(Player &p){
    if(getRandomNumber(0, 2)){
        p.Hill();
        std::cout << "-> You have bonus health" << std::endl;
    }
}

int main(){
    std::cout << "Enter your name: ";
	std::string playerName;
	std::cin >> playerName;
 
	Player p(playerName);
	std::cout << "Welcome, " << p.getName() << "\n";
 
	std::cout << "You have " << p.getHealth() << " health and are carrying " << p.getGold() << " gold\n";

	srand(static_cast<unsigned int>(time(0)));
	rand();

    bool flag = true;
    while (flag){
        Monster m = Monster::getRandomMonster();
        std::cout << "You have encountered a " << m.getName() << "(" << m.getSymbol() << ")\n";

        flag = fightMonster(m, p);
        if (p.hasWon()){
            std::cout << "Congratulations You WIN!!!";
            break;
        }
    }
	return 0;
}
