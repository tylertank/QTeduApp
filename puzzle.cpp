// Name: Jarrod King, Tyler Harkness, Axe Tang, Sam Farrell, Coltin Thiede
// Class: CS3505
// Assignment: A8
#include "puzzle.h"

///
/// \brief Puzzle::Puzzle - the constructor for the puzzle class. It will build the puzzle and solve it for the minimum spanning tree
/// \param nodes - this is the number of the nodes the puzzle should have
/// \param seed - this is a random seed used to generate random edges
///
Puzzle::Puzzle(int nodes, int seed)
{


    srand(time(nullptr) - 100-seed);
    this->nodes = nodes;
    int connectivity = 3;
    int maxWeight = 20;
    int minWeight = 1;
    for (int i=0;i<nodes;i++) {
        weights.push_back(std::vector<int>());
        weights.at(i).assign(nodes, 0);
    }
    std::list<int> connected;
    std::list<int> unused;
    for (int i=0;i<connectivity;i++) {
        connected.clear();
        unused.clear();
        connected.push_back(0);
        for (int vertex = 1;vertex < nodes;vertex++) {
            unused.push_back(vertex);
        }
        while (unused.size() > 0) {
            int weight = rand() % (maxWeight-minWeight + 1) + minWeight;
            auto c_front = connected.begin();
            std::advance(c_front, rand() % connected.size());
            int fromVertex = *c_front;
            auto u_front = unused.begin();
            std::advance(u_front, rand() % unused.size());
            int toVertex = *u_front;
            weights.at(fromVertex).at(toVertex) = weight;
            weights.at(toVertex).at(fromVertex) = weight;
            connected.push_back(toVertex);
            unused.remove(toVertex);
        }
    }

    // #############################################################################
    // #                    FINDING THE MINIMUM SPANNING TREE                      #
    // #############################################################################

    auto cmp = [](std::tuple<int, int, int>left , std::tuple<int, int, int>right) {return std::get<2>(left) > std::get<2>(right);};
    std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, decltype(cmp)> prioQ(cmp);
    // Jarnik's algorithm
    std::vector<bool> vertices;
    int sum = 0;
    vertices.assign(nodes, false);
    vertices.at(0) = true;
    // add all initial edges to prioQ
    for (int i=0;i < nodes;i++) {
        int w = weightAt(0, i);
        if (w != 0) {
            prioQ.push(std::tuple<int, int, int>(0, i, w));
        }
    }
    for (int j = 1;j < nodes;j++) {
        // pull the next edge

        std::tuple<int, int, int> nextEdge = prioQ.top();
        prioQ.pop();
        // If both root and child are already visited, skip this edge and reset j
        if (vertices.at(std::get<0>(nextEdge)) && vertices.at(std::get<1>(nextEdge))) {
            j--;
            continue;
        }
        else {
            int newVertex = vertices.at(std::get<0>(nextEdge)) ? std::get<1>(nextEdge): std::get<0>(nextEdge);
            vertices.at(newVertex) = true;
            sum += std::get<2>(nextEdge);

            // add all next edges to prioQ
            for (int i=0;i < nodes;i++) {
                int w = weightAt(newVertex, i);
                if (w != 0) {
                    prioQ.push(std::tuple<int, int, int>(newVertex, i, w));
                }
            }
        }
    }
    minSpanSum = sum;


}

///
/// \brief Puzzle::getSize - Returns the number of nodes in the puzzle
/// \return - returns an int representing the number of nodes
///
int Puzzle::getSize() {
    return nodes;
}

///
/// \brief Puzzle::getMinTreeValue - returns the weight of the minimum spanning tree
/// \return - an int representing the total edge weight of a the minimum spanning tree
///
int Puzzle::getMinTreeValue() {
    return minSpanSum;
}
///
/// \brief Puzzle::weightAt - this gets the weight of the edge connecting nodes
/// \param x - this is the first node
/// \param y - this is the second node
/// \return - returns the weight between the two nodes input as parameters
///
int Puzzle::weightAt(int x, int y) {
    return weights.at(x).at(y);
}
