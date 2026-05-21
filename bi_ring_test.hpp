#include "bi_ring.hpp"
#include <iostream>
#include <string>
#include <map>
//External functions-------------------------------------------------------------
template<typename Key, typename Info>
bi_ring<Key, Info> join(const bi_ring<Key, Info>& first, const bi_ring<Key, Info>& second) 
{
    bi_ring<Key, Info>result;
    std::map<Key, Info>result_map; 

    auto process_ring = [&](const bi_ring<Key, Info>& ring)
    {
        if (ring.empty()) return;
        
        auto it = ring.cbegin();
        size_t initial_count = ring.size(); 

        for (size_t i = 0; i < initial_count; ++i)
        {  
            auto pair = *it;

            if (result_map.count(pair.first))
            {
                result_map[pair.first] = result_map[pair.first] + pair.second;
            } 
            else
            {
                result_map[pair.first] = pair.second;
            }
            ++it;
        }
    };

    process_ring(first);
    process_ring(second);

    for(const auto& pair : result_map)
    {
        result.push_back(pair.first, pair.second);
    }
    return result;
}
//---------------------------------------------------------------------------------------
template<typename Key, typename Info>
bi_ring<Key, Info> shuffle (const bi_ring<Key, Info>& first, unsigned int fcnt,
    const bi_ring<Key, Info>& second, unsigned int scnt, unsigned int reps)
{
    bi_ring<Key, Info> result;

    if (first.empty() && second.empty()) return result;

    //Get const iterators for safe, circular traversal
    auto it1 = first.cbegin();
    auto it2 = second.cbegin();

    for (unsigned int r = 0; r<reps; ++r)
    {
        for (unsigned int i = 0; i < fcnt; ++i)
        {
            if (first.empty()) break;
            
            auto pair = *it1;
            result.push_back(pair.first, pair.second);
            ++it1;
        }
        for (unsigned int i = 0; i < scnt; ++i)
        {
            if (second.empty()) break;
            
            auto pair = *it2;
            result.push_back(pair.first, pair.second);
            ++it2;
        }
    } 
    return result;
}
//------------------------------------------------------------
template <typename Key, typename Info>
void replace (bi_ring<Key, Info>& ring1, bi_ring<Key, Info>& ring2)
{
   auto find_cut_point = [](bi_ring<Key, Info>& ring) -> typename bi_ring<Key, Info>::iterator
   {
       size_t ring_size = ring.size();

       if (ring_size == 0) 
            return ring.end()

       size_t first_half_size= ring_size/2;
        //start from the beginning
       auto cut_it = ring.begin();
        //makes the iterator point to the first element of second half
       for (size_t i = 0; i < first_half_size; ++i)
       {
           ++cut_it;
       }
       return cut_it;
   };
   
   typename bi_ring<Key, Info>::iterator cut1 = find_cut_point(ring1);
   typename bi_ring<Key, Info>::iterator cut2 = find_cut_point(ring2);
   //storing sec half
   size_t half1_size =ring1.size()- (ring1.size() / 2);
   size_t half2_size =ring2.size() - (ring2.size() / 2);

   for (size_t i = 0; i < half1_size; ++i)
   {
       //get el at second half
       const Key&k = (*cut1).first;
       const Info&v = (*cut1).second;
  
       ring2.push_back(k, v);
       cut1 = ring1.erase(cut1);
       if (ring1.empty())
            break;
   }

   for (size_t i = 0; i < half2_size; ++i)
   {

       const Key& k = (*cut2).first;
       const Info& v = (*cut2).second;
\
       ring1.push_back(k, v);
       cut2= ring2.erase(cut2);
       if(ring2.empty()) 
            break;
   }
}
   //------------------------------------------
using Ring = bi_ring<int, std::string>;
using RingStrInt = bi_ring<std::string, int>;

