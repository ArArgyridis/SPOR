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

#include "functions.h"
#include <iostream>
#include <fstream>

using namespace std;

void binomialArray(int n, matrix2dPtr input, matrix2dPtr output) {

        default_random_engine generator;
        boost::mt19937 rng;
     for ( iterator1 it1=input->begin1(); it1 !=input->end1(); it1++ ) {

         for (iterator2 it2 = it1.begin(); it2 != it1.end(); it2++) {

            boost::random::binomial_distribution<> my_binomial(n, *it2);
            boost::random::variate_generator<boost::mt19937&, boost::random::binomial_distribution<>  >next_value(rng, my_binomial);
           (*output)(it2.index1(), it2.index2()) = next_value();

            /*
            std::binomial_distribution<int> distribution(n, *it2 );
            (*output)(it2.index1(), it2.index2()) = distribution(generator);
            */
          }
        }
}

void printMatrix(string str, matrix2dPtr mat) {

    cout <<"Array: " << str << endl;
    if (mat == NULL)
        cout <<" NULL ARRAY\n";
    else {
        for (iterator1 it1 = mat->begin1(); it1 !=mat->end1(); it1++) {
            for (iterator2 it2 = it1.begin(); it2 != it1.end(); it2++)
                cout << *it2 << "\t";

            cout <<endl;
        }

        cout << endl;
    }
}

void writeMatrix(string filename, matrix2dPtr mat) {

    ofstream file (filename);
    if (mat == NULL)
        file <<" NULL ARRAY\n";
    else {
        int i = 0;
        for (iterator1 it1 = mat->begin1(); it1 !=mat->end1(); it1++) {
            file << ++i << "\t";
            for (iterator2 it2 = it1.begin(); it2 != it1.end(); it2++)
                file <<*it2 <<"\t";
            file << endl;
        }
    }
    file.close();
}


double computeAverage(matrix2dPtr input) {
   double out = 0.0;
    for ( iterator1 it1=input->begin1(); it1 !=input->end1(); it1++ ) {
       for (iterator2 it2 = it1.begin(); it2 != it1.end(); it2++) {
           out += *it2;
       }
    }
    return out/(input->size1()*input->size2());
}

void hyperbolicTangentArray(matrix2dPtr input, matrix2dPtr output) {

    for ( iterator1 it1=input->begin1(); it1 !=input->end1(); it1++ ) {
       for (iterator2 it2 = it1.begin(); it2 != it1.end(); it2++) {
           (*output)(it2.index1(), it2.index2()) = tanh (*it2);
       }
    }

}

void logArray(matrix2dPtr input, matrix2dPtr output) {

    for ( iterator1 it1=input->begin1(); it1 !=input->end1(); it1++ ) {
       for (iterator2 it2 = it1.begin(); it2 != it1.end(); it2++) {
           (*output)(it2.index1(), it2.index2()) = log (*it2);
       }
    }

}

void exponentialArray(matrix2dPtr input, matrix2dPtr output) {

     for ( iterator1 it1=input->begin1(); it1 !=input->end1(); it1++ ) {
        for (iterator2 it2 = it1.begin(); it2 != it1.end(); it2++) {
            (*output)(it2.index1(), it2.index2()) = exp (*it2);
        }
     }
}

void uniformArray(matrix2dPtr input, double min, double max) {
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(min, max);

     for ( iterator1 it1=input->begin1(); it1 !=input->end1(); it1++ )
         for (iterator2 it2 = it1.begin(); it2 != it1.end(); it2++)
             *it2 = distribution(generator);

}

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

void sigmoidArray(matrix2dPtr input, matrix2dPtr output) {

     for ( iterator1 it1=input->begin1(); it1 !=input->end1(); it1++ ) {
       for (iterator2 it2 = it1.begin(); it2 != it1.end(); it2++) {
           (*output)(it2.index1(), it2.index2()) = sigmoid (*it2);
       }
     }
}

/*
void printMatrix(string str, matrix2dPtr mat) {

    cout <<"Array: " << str << endl;
    if (mat == NULL)
        cout <<" NULL ARRAY\n";
    else {
        for (iterator1 it1 = mat->begin1(); it1 !=mat->end1(); it1++) {
            for (iterator2 it2 = it1.begin(); it2 != it1.end(); it2++)
                cout << *it2 << "\t";

            cout <<endl;
        }

        cout << endl;
    }
}

*/
