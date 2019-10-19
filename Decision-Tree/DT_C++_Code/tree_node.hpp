#ifndef TREE_NODE_HPP_INCLUDE
#define TREE_NODE_HPP_INCLUDE

#include <fstream>
#include <memory>

struct TreeNode {
  TreeNode(int val);
  TreeNode(int attr, int split_point);
  void serialize(std::ofstream &out);
  int attr, split_point, val, type;   // attr - attribute    // split_point - 0 or 1 // val - + or -node // type - leaf node
  int noofzeroclass, noofoneclass;
  std::shared_ptr<TreeNode> left_child, right_child;
};

#endif /* end of include guard: TREE_NODE_HPP_INCLUDE */

