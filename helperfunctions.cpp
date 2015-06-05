 /*
    Part of the code of the SPatial Ontology Reasoner designed to reason over multi-scale GEOBIA Ontologies, as described in the following paper:
    Argyridis A., Argialas, D., 2015. A Fuzzy Spatial Reasoner for Multi-Scale GEOBIA Ontologies, Photogrammetric Engineering and Remote Sesing, 41-48

    Copyright (C) 2015  Argyros Argyridis

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
 
#include "helperfunctions.h"
#include <sstream>


using namespace std;



vector<int> getCommonElements(vector<int> first, vector<int> second) {

    vector<int> v;
    vector<int>::iterator it;
    sort (first.begin(), first.end());
    sort (second.begin(), second.end());
     it=std::set_intersection (first.begin(), first.end(), second.begin(), second.end(), v.begin());

     v.resize(it-v.begin());
    return v;
}

string genRandomString( const int len) {
    string alphanum =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    stringstream strm;

    for (int i = 0; i < len; ++i) {
        strm << ( alphanum.at (  rand() % ( alphanum.length() - 1)  ) ) ;
    }
   return strm.str() ;
}
