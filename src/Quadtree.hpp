#ifndef __QUADTREE_HPP__
#define __QUADTREE_HPP__

#include "Block.hpp"

using namespace std;

class Node {
    public:
        // Attributes
        Block block;
        Node* child[4];
        bool isLeaf;
        // Static attributes
        static int varianceMethod; // Encodings
        static double varianceThreshold;
        static int minBlockSize; 
        // Constructor
        Node(int x, int y, int width, int height, const vector<vector<vector<int>>>* rgbMatrix);
        // Methods
        bool isValidConstraints();
};

class Quadtree {  
    private:
        Node* root;
    public:
        // Constructor
        Quadtree(int widht, int height, const vector<vector<vector<int>>>* rgbMatrix, int varianceMethod, double varianceThreshold, int minBlockSize);
        // Methods
        void compressImage();
};

#endif
