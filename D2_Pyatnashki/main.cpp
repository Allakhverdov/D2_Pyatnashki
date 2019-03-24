//
//  D2 пятнашки часть б
//
#include<iostream>
#include<string>
#include<vector>
#include<stack>
struct Position{
    Position() = default;
    Position& operator = (const Position& x1);
    Position(const Position& x);
    Position(std::vector<int>Square, int digit, int step): Square(Square), digit(digit), step(step){} // количество уже сделанных ходов запишем в step
    void GetNeighbour(std::vector<Position>&Neighbour, std::vector<std::string>&Side);
    std::vector<int> Square;
    int evrist = 0;
    int step = 0;
    void Evrist_();
private:
    int digit = 0;
    std::vector<int> GetL();
    std::vector<int> GetR();
    std::vector<int> GetU();
    std::vector<int> GetD();
    int G();
    int Corner_conflict();
    int Linear_conflict();
    int ManhattanDistance();
    bool operator () (const Position& x, const Position& y) const;
};

Position::Position(const Position& x)
{
    this->step = x.step;
    this->Square = x.Square;
    this->digit = x.digit;
    this->evrist = x.evrist;
}

Position& Position::operator = (const Position& x1)
{
    if(&x1 == this)
    {
        return *this;
    }
    this -> step = x1.step;
    this -> Square = x1.Square;
    this -> digit = x1.digit;
    this -> evrist = x1.evrist;
    return *this;
}

const bool operator < (const Position& x, const Position& y)
{
    if(x.evrist < y.evrist)
    {
        return false;
    }
    return true;
}

bool Position::operator () (const Position& x, const Position& y) const
{
    return x < y;
}

struct Cmp
{
    bool operator () (const Position& x, const Position& y)
    {
        return x.evrist > y.evrist;
    }
};

std::vector<int> Position::GetL()                                       //сдвиг влево
{
    std::vector<int>Square_ = Square;
    std::swap(Square_[digit], Square_[digit - 1]);
    return Square_;
}

std::vector<int> Position::GetR()                                       //сдвиг вправо
{
    std::vector<int>Square_ = Square;
    std::swap(Square_[digit], Square_[digit + 1]);
    return Square_;
}

std::vector<int> Position::GetU()                                       //сдвиг вверх
{
    std::vector<int>Square_ = Square;
    std::swap(Square_[digit], Square_[digit - 4]);
    return Square_;
}

std::vector<int> Position::GetD()                                       //сдвиг вниз
{
    std::vector<int>Square_ = Square;
    std::swap(Square_[digit], Square_[digit + 4]);
    return Square_;
}

void Position::GetNeighbour(std::vector<Position>&Neighbour, std::vector<std::string>&Side)         //получаем всех соседей
{
    if(digit % 4 != 0)
    {
        std::vector<int> LNeighb = GetL();
        Position LN(LNeighb, digit - 1, step + 1);                                          //конструируем положение
        LN.Evrist_();                                                                       //считаем эвристику
        Neighbour.push_back(LN);
        Side.push_back("R");
    }
    if(digit % 4 != 3)
    {
        std::vector<int> LNeighb = GetR();
        Position RN(LNeighb, digit + 1, step + 1);
        RN.Evrist_();
        Neighbour.push_back(RN);
        Side.push_back("L");
    }
    if(digit > 3)
    {
        std::vector<int> UNeighb = GetU();
        Position UN(UNeighb, digit - 4, step + 1);
        UN.Evrist_();
        Neighbour.push_back(UN);
        Side.push_back("D");
    }
    if(digit < 12)
    {
        std::vector<int> DNeighb = GetD();
        Position DN(DNeighb, digit + 4, step + 1);
        DN.Evrist_();
        Neighbour.push_back(DN);
        Side.push_back("U");
    }
}

int Position::ManhattanDistance()                                   //манхеттенское расстояние
{
    int evrist = 0;
    for(int i = 0; i < 16; ++i)
    {
        if(Square[i] != 15)
        {
            int real_x = Square[i] / 4;
            int real_y = Square[i] % 4;
            int x = i / 4;
            int y = i % 4;
            evrist += abs(real_x - x) + abs(real_y - y);
        }
    }
    return evrist;
}

int Position::Linear_conflict()                             //линейный конфликт
{
    int evrist = 0;
    for(int i = 0; i < 4; ++i)
    {
        for(int j = i * 4; j < (i + 1) * 4; ++j)
        {
            if(Square[j] != 15 && Square[j]/4 == j/4)
            {
                for(int p = j % 4 + 1; p < 4; ++p)
                {
                    if(Square[4 * i + p]/4 == j/4 && Square[j] > Square[4 * i + p])
                    {
                        evrist += 3;
                    }
                }
            }
        }
    }
    for(int i = 0; i < 4; ++i)
    {
        for(int j = i; j < 16; j += 4)
        {
            if(Square[j] != 15 && Square[j] % 4 == j % 4)
            {
                for(int p = j + 4; p < 16; p += 4)
                {
                    if(Square[p] % 4 == p % 4 && Square[j] > Square[p])
                    {
                        evrist += 2;
                    }
                }
            }
        }
    }
    return evrist;
}

int Position::Corner_conflict()                                         //угловой конфликт
{
    int evrist = 0;
    if(Square[1] == 1 && Square[4] == 4 && Square[0] != 0)
    {
        evrist += 1;
    }
    if(Square[2] == 2 && Square[7] == 7 && Square[3] != 3)
    {
        evrist += 1;
    }
    if(Square[8] == 8 && Square[13] == 13 && Square[12] != 12)
    {
        evrist += 1;
    }
    return evrist;
}



void Position::Evrist_()                                                //подсчёт эвристики
{
    evrist = ManhattanDistance() + Linear_conflict() + Corner_conflict();
}

bool is_goal (Position node)                                                //проверяем не дошли ли мы до нужного положения
{
    std::vector<int> standard = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    if(node.Square == standard)
    {
        return true;
    }
    return false;
}

int search (Position node, Position last, int step, int& bound,std::stack<std::string>& Side)           //функция использующаяся внутри IDA
{
    int evrist = step + node.evrist;                                            //считаем конечную эвристику
    if(evrist > bound)                                                          //если эвристика больше нынешней глубины, возвращаем её
    {
        return evrist;
    }
    if(is_goal (node))                                                          //если дошли до конца, то возвращаем -1
    {
        return -1;
    }
    std::vector<Position>Children;
    std::vector<std::string>Side_;
    node.GetNeighbour(Children, Side_);
    int min = 999999;
    for(int i = 0; i < Children.size(); ++i)                                    //иначе проходим всех детей
    {
        if(Children[i].Square != last.Square)                                   //если ребенок не равен отцу то делаем следующее
        {
            int path_length = search (Children[i], node, step + 1, bound, Side);    //запускаемся от этого ребенка
            if(path_length == -1)
            {                                                                       //если получили конечную раскладку
                Side.push(Side_[i]);                                                //вочтанавливаем путь
                return -1;
            }
            if(path_length < min)                                                   //иначе
            {
                min = path_length;                                                  //увеличиваем миниму, который скажет как глубоко шагать на следующей итерации
            }
        }
    }
    return min;
}

std::stack<std::string> IDA (Position root)
{
    int bound = root.evrist;
    std::stack<std::string> Side;
    while(true)
    {
        bound = search(root , root, 0, bound, Side);                                 //шагаем пока не получим результат, после итерации увеличиваем глубину
        if(!Side.empty())
        {
            return Side;
        }
    }
}

int main()
{
    int digit = 0;
    std::vector<int>A_;
    std::vector<int> standard = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    std::vector<int>chenge;
    for(int i = 0; i < 16; ++i)                             //читаем
    {
        int cell = 0;
        std::cin >> cell;
        if(cell == 0)
        {
            digit = i;
        }
        A_.push_back((cell + 15) % 16);                     //приводим в удобный вид
        if(cell != 0)
        {
            chenge.push_back(cell);
        }
    }
    if(A_ == standard)
    {
        std::cout << 0;
        return 0;
    }
    Position A(A_, digit, 0);
    int chen = 0;
    for(int i = 0; i < 15; ++i)                                 //проверяем на решаемость
    {
        for(int j = i + 1; j < 15; ++j)
        {
            if(chenge[i] > chenge[j])
            {
                ++chen;
            }
        }
    }
    if((chen + digit/4 + 1) % 2 == 1)
    {
        std::cout << -1;
    }
    else
    {
        A.Evrist_();
        std::stack<std::string> answer =  IDA (A);                  //выводим ответ
        std::cout << answer.size()<< std::endl;
        while(!answer.empty())
        {
            std::cout << answer.top();
            answer.pop();
        }
    }
    return 0;
}
