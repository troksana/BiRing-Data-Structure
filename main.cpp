#include <iostream>
#include "bi_ring.hpp"
#include "bi_ring_test.hpp"

using Ring=bi_ring<int,std::string>;
using RingStrInt=bi_ring<std::string,int>;

//External functions for tests----------------------------
void check(bool condition, const std::string& msg)
{
    if (condition)
    {
        std::cout<<"PASS \t"<<msg<<"\n";
    }
    else
    {
        std::cout<<"FAIL \t"<<msg<<"\n";
    }
}
std::string ring_to_string(const RingStrInt& r)
{
    if (r.empty()) 
        return "[]";
    std::string s = "[";
    auto it= r.cbegin();
    size_t count= r.size();
    for(size_t i= 0; i < count; ++i)
    {
        s += (*it).first + ":" + std::to_string((*it).second);
        ++it;
        if (i < count - 1) 
            s += ", ";
    }
    s += "]";
    return s;
}
//-------------------------------------------------------------------
int main()
{
    std::cout<<"---------------TESTING bi_ring-------------------\n\n";
    std::cout<<"---------Empty ring--------\n";
    {
        Ring r;
        check(r.empty(), "Empty ring initially");
        check(r.size() == 0, "Empty ring size == 0");
        check(r.begin() == r.end(), "begin() == end() on empty ring");
    }

   std::cout<<"\n---------Push front (one element)--------\n";
    {
        Ring r;
        r.push_front(10, "Eagle");

        check(!r.empty(), "push_front makes ring non-empty");
        check(r.size() == 1, "push_front size == 1");
        check((*r.begin()).first == 10, "push_front inserted correct key");
        check((*r.begin()).second == "Eagle", "push_front inserted correct info");
    }
   std::cout<<"\n---------Push front (more elements)--------\n";
    {
        Ring r;
        r.push_front(1, "Eagle");
        r.push_front(2, "Wolf");
        r.push_front(3, "Mouse");

        auto it = r.begin();
        check((*it).first == 3, "push_front order test 1");
        ++it;
        check((*it).first == 2, "push_front order test 2");
        ++it;
        check((*it).first == 1, "push_front order test 3");
    }
   std::cout<<"\n---------Push back (more elements)--------\n";
    {
        Ring r;
        r.push_back(1, "Monkey");
        r.push_back(2, "Parrot");
        r.push_back(3, "Gorilla");

        auto it = r.begin();
        check((*it).first == 1, "push_back order test 1");
        ++it;
        check((*it).first == 2, "push_back order test 2");
        ++it;
        check((*it).first == 3, "push_back order test 3");
    }

    std::cout<<"\n---------Pop front---------\n";
    {
        Ring r;
        r.push_back(1, "Alpaca");
        r.push_back(2, "Donkey");

        auto it = r.pop_front();
        check((*it).first == 2, "pop_front moves front");
        check(r.size() == 1, "pop_front decrements size");
    }

    std::cout<<"\n---------Push back---------\n";
    {
        Ring r;
        r.push_back(1, "Crab");
        r.push_back(2, "Sheep");

        auto it = r.pop_back();
        check((*it).first == 1 && (*it).second == "Crab", "pop_back moves back");
        check(r.size() == 1, "pop_back decrements size");
    }

    std::cout<<"\n---------Insert ---------\n";
    {
        Ring r;
        r.push_back(1, "Cow");
        r.push_back(3, "Chicken");

        auto pos = r.begin();
        ++pos;

        r.insert(pos, 2, "Deer");//In between insertion

        auto it = r.begin();
        check((*it).first == 1, "insert middle test pos=1");
        ++it;
        check((*it).first == 2, "insert middle test pos=2");
        ++it;
        check((*it).first == 3, "insert middle test pos=3");
    }
    std::cout<<"\n---------Erase ---------\n";
    {
        Ring r;
        r.push_back(1, "Cheetah");
        r.push_back(2, "Ferret");
        r.push_back(3, "Seagull");

        auto it = r.begin();
        ++it;
        r.erase(it);

        check(r.size() == 2, "erase reduces size");

        auto t = r.begin();
        check((*t).first == 1, "erase keeps first");
        ++t;
        check((*t).first == 3, "erase removes correct middle node");
    }

    std::cout<<"\n---------Erase_all ---------\n";
    {
        Ring r;
        r.push_back(1, "Octopus");
        r.push_back(2, "Lobster");
        r.push_back(1, "Dolphin");
        r.push_back(3, "Sea lion");

        size_t removed = r.erase_all(1);
        check(removed == 2, "erase_all removes correct amount");
        check(r.size() == 2, "erase_all new size correct");

        auto it = r.begin();
        check((*it).first == 2, "erase_all remaining node 1");
        ++it;
        check((*it).first == 3, "erase_all remaining node 2");
    }

    std::cout<<"\n---------Copy constructor ---------\n";
    {
        Ring r;
        r.push_back(1, "Gazelle");
        r.push_back(2, "Giraffe");

        Ring copy(r);

        auto it1 = r.begin();
        auto it2 = copy.begin();

        check((*it1).first == (*it2).first, "copy constructor key1");
        ++it1; ++it2;
        check((*it1).first == (*it2).first, "copy constructor key2");
    }

    std::cout<<"\n---------Assignment operator ---------\n";
    {
        Ring r1;
        r1.push_back(1, "Koala");
        r1.push_back(2, "Lemur");

        Ring r2;
        r2.push_back(9, "Warthdog");

        r2 = r1;

        check(r2.size() == 2, "assignment operator size");

        auto it1 = r1.begin();
        auto it2 = r2.begin();

        check((*it1).first == (*it2).first, "assignment copy key1");
        ++it1; ++it2;
        check((*it1).first == (*it2).first, "assignment copy key2");
    }

    std::cout<<"\n---------Iterator wrap around++ ---------\n";
    {
        Ring r;
        r.push_back(1, "Kangaroo");
        r.push_back(2, "Zebra");
        r.push_back(3, "Wildebeest");

        auto it = r.begin();
        ++it; // 2
        ++it; // 3
        ++it; // wrap -> 1

        check((*it).first == 1, "iterator wrap-around forward");
    }

    std::cout<<"\n---------Iterator wrap around-- ---------\n";
    {
        Ring r;
        r.push_back(1, "Ladybug");
        r.push_back(2, "Bee");
        r.push_back(3, "Moth");

        auto it = r.begin(); // at 1
        --it;                // should wrap to 3

        check((*it).first == 3, "iterator wrap-around backward");
    }

    std::cout << "\n---------External Function: join---------\n";

    RingStrInt r1;
    r1.push_back("Seal", 1);
    r1.push_back("Whale", 1);
    r1.push_back("Fish", 2);
    r1.push_back("Starfish", 1);

    RingStrInt r2;
    r2.push_back("Whale", 1);
    r2.push_back("Fish", 1);
    r2.push_back("Starfish", 3);
    r2.push_back("Shrimp", 5);

    //Expected: [Shrimp:5, Whale:2, Starfish:4, Fish:3, Seal:1]
    RingStrInt result = join(r1, r2);
    
    //Check size
    check(result.size() == 5, "join: Size check (Expected 5)");

    //Check content and sums
    std::map<std::string, int> actual_map;
    auto it = result.cbegin();
    for (size_t i = 0; i < result.size(); ++i)
    {
        actual_map[(*it).first] = (*it).second;
        ++it;
    }

    check(actual_map["Seal"] == 1, "join: Sum for 'seal' (1) is correct");
    check(actual_map["Whale"] == 2, "join: Sum for 'whale' (1+1=2) is correct");
    check(actual_map["Fish"] == 3, "join: Sum for 'fish' (2+1=3) is correct");
    check(actual_map["Starfish"] == 4, "join: Sum for 'starfish' (1+3=4) is correct");
    check(actual_map["Shrimp"] == 5, "join: Sum for 'shrimp' (5) is correct");
    std::cout << "Output: " << ring_to_string(result) << "\n";
    std::cout << "Expected Output (Key-Sorted): [Shrimp:5, Shale:2, Starfish:4, Fish:3, Seal:1]\n";
    //Joining with empty ring
    RingStrInt r_empty;
    RingStrInt result_empty = join(r_empty, r1);
    check(result_empty.size() == 4, "join: Joining with empty ring preserves non-empty size");

    std::cout << "\n---------External Function: shuffle---------\n";

    //First=>[Uno:1, Due:2, Tre:3, Quattro:4]
    RingStrInt ri1;
    ri1.push_back("Uno", 1);
    ri1.push_back("Due", 2);
    ri1.push_back("Tre", 3);
    ri1.push_back("Quattro", 4);

    //Second=>[Bir:1, Iki:2, Uc:3, Dort:4, Bes:5]
    RingStrInt ri2;
    ri2.push_back("Bir", 1);
    ri2.push_back("Iki", 2);
    ri2.push_back("Uc", 3);
    ri2.push_back("Dort", 4);
    ri2.push_back("Bes", 5);

    RingStrInt result2 = shuffle(ri1, 1, ri2, 2, 3);
    //Expected: [Uno:1, Bir:1, Iki:2, Due:2, Uc:3, Dort:4, Tre:3, Bes:5, Bir:1]

    check(result2.size() == 9, "shuffle: Size check (Expected 9)");

    std::string expected_sequence = "Uno:1, Bir:1, Iki:2, Due:2, Uc:3, Dort:4, Tre:3, Bes:5, Bir:1";
    std::string actual_sequence = ring_to_string(result2);

    //For clean comparison, erase [ ] brackets
    if (actual_sequence.length() > 2)
    {
         actual_sequence = actual_sequence.substr(1, actual_sequence.length() - 2);
    }
    
    check(actual_sequence == expected_sequence, "shuffle: Sequence matches expected pattern");
    
    std::cout << "Output: " << actual_sequence << "\n";
    std::cout << "Expected Output: " << expected_sequence << "\n";
    
    // Test Case: r1 is empty
    RingStrInt ri_empty;
    RingStrInt result_empty1 = shuffle(ri_empty, 1, ri2, 1, 2); //Expected: [Bir:1, Iki:2]
    check(result_empty1.size() == 2, "Shuffle: r1 empty, skips r1 blocks");

    // Test Case: r2 is empty
    RingStrInt result_empty2 = shuffle(ri1, 1, ri_empty, 1, 2); //Expected: [Uno:1, Due:2]
    check(result_empty2.size() == 2, "Shuffle: r2 empty, skips r2 blocks");

    std::cout << "\n---------External Function: replace---------\n";
    RingStrInt r1;
    for (int i = 0; i <= 9; ++i) 

    {
        r1.push_back(std::to_string(i), i);
    }
    RingStrInt r2;
    for (int i = 10; i <= 22; ++i)
    {
        r2.push_back(std::to_string(i), i);
    }
    replace(r1, r2);
    std::string expected1 = "0:0, 1:1, 2:2, 3:3, 4:4, 16:16, 17:17, 18:18, 19:19, 20:20, 21:21, 22:22";

    std::string actual1 = ring_to_string(r1);

    if(actual1.length() > 2) actual1 = actual1.substr(1, actual1.length() - 2);

    check(actual1 == expected1, "replace: r1 content matches expected");
    check(r1.size() == 12, "replace: r1 size check (Expected 12)");

    std::string expected2 = "10:10, 11:11, 12:12, 13:13, 14:14, 15:15, 5:5, 6:6, 7:7, 8:8, 9:9";
    std::string actual2 = ring_to_string(r2);

    if (actual2.length() > 2) actual2 = actual2.substr(1, actual2.length() - 2);

    check(actual2 == expected2, "replace: r2 content matches expected");
    check(r2.size() == 11, "replace: r2 size check (Expected 11)");

    std::cout << "Ring 1 Output: " << actual1 << "\n";
    std::cout << "Ring 2 Output: " << actual2 << "\n";
    
    std::cout << "\n-------------------------End Of The Tests-------------------------\n";
    return 0;
}