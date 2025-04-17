#ifndef SET_H
#define SET_H

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;


class set {
    public:
      vector<int> data;
      //constructors
      set() {
      
      }
  
      set(vector<int> arr) {
        for(auto a : arr) {
          add(a);
        }
      }
  
      //methods
  
      //checks if a given integer is in the set
      bool isIn(int a) {
        for(auto i : data){
          if(i == a){
            return true;
          }
        }
        return false;
      }
  
  
      //returns if the set is empty
      bool isEmpty() {
        return data.size() == 0;
      }
  
      //returns size of set
      int size() {
        return data.size();
      }
  
      //adds a given integer if it's not in the set
      bool add(int a) {
        if(!isIn(a)) {
          data.push_back(a);
          return true;
        }
        return false;
      }
  
  
      //finds intersection of given set and current set
      set intersection(set& other) {
        set out;
        for(auto i : data) {
          if (other.isIn(i))
            out.add(i);
          
        }
        return out;
      }
  
  
      //find the union of two sets
      set onion(set& other) {
        set out;
        for(auto i : other.data){
          out.add(i);
        }
        return out;
      }
  
  
      //finds the difference between two sets (what is in the first but 
      //not in the second)
      set difference(set& other) {
        set out;
        for(auto i : data) {
          if(!other.isIn(i)) {
            out.add(i);
          }
        }
        return out;
      }
  
  
      //operator overloads
      
      //printing
      friend ostream& operator<<(ostream& out, set& s) {
        for(auto i : s.data){
          out << i << " ";
        }
        out << endl;
        return out;
      }
        
      //checking if in set with item to check second
      friend bool operator^(set& s, int check){
        return s.isIn(check);
      }
  
      //checking if in set with item to check first
      friend bool operator^(int check, set& s){
        return s.isIn(check);
      }
  
      //adding to set
      set& operator+=(int addl) {
        add(addl);
        return *this;  //allows chaining
      }
  
      //intersection
      set operator*(set& other) {
        return intersection(other);
      }
  
      //union
      set operator+(set& other) {
        return onion(other);
      }
  
      //difference
      set operator-(set& other) {
        return difference(other);
      }
  
      //empty
      friend bool operator!(set& current) {
        return current.isEmpty();
      }
  
      //subset
      friend bool operator<=(set& current, set& other) {
        for(auto i : current.data) {
          if (!(i^other)) return false;
        }
        return true;
      }
  
      //proper subset
      friend bool operator<(set& current, set& other) {
        if (current.size() >= other.size()) return false;
        for (auto i : current.data) {
          if(!(i ^ other)) return false;
        }
        return true;
      }
  
};

#endif