#include "decision_tree.hpp"
#include "tree_node.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <unordered_set>
#include <cmath>

using namespace std;

namespace {
  const constexpr int ATTR_SIZE = 500; //24
  const constexpr double MAX_INFO = 1;

  bool split_examples(int attr, int split, const vector<int> &example);
}

shared_ptr<TreeNode> DecisionTree::gen_tree(vector<vector<int>> &examples) {
  //examples_size = examples.size();
    std::vector<bool> used_attr;
    if(examples[0].size() > 0)
        used_attr.assign(examples[0].size(),false);
    else
        return nullptr;
  return gen_tree(nullptr, examples.begin(), examples.end(), used_attr);
}

shared_ptr<TreeNode> DecisionTree::gen_tree(
    shared_ptr<TreeNode> parent,
    const vector<vector<int>>::iterator &begin_it,
    const vector<vector<int>>::iterator &end_it,
             vector<bool> used_attr,
    int depth
  ) {
  int attr, split_point;
  size_t size = end_it - begin_it; // Size of current examples
  vector<vector<int>>::iterator split_it;
  shared_ptr<TreeNode> node;
  double info, child_info;
  // No more example
  if(begin_it == end_it) {
    return nullptr;
  }
  // Get entropy
    if(bvariance)
        info = variance(begin_it, end_it);
    else
        info = entropy(begin_it, end_it);

  //cout << "Depth: " << depth << ", Data size: " << size << ", Info: " << info << endl;

  // It's exactly split into two part
  if(0 >= info) {
    // Return answer
    return make_shared<TreeNode>((*begin_it)[ATTR_SIZE]); //24
  }
    
  tie(attr, split_point, child_info) = choose_attr(begin_it, end_it, split_it, used_attr);
    
  // Check if it can't split into two part
  if(attr == -1 || info <= child_info /*|| 0.1 >= info*/) {
    // Count of 1 to decide answer
   size_t ans_1 = count_if(begin_it, end_it, [](const vector<int> &example) {
        return example[ATTR_SIZE] == 1; //24
    });
     
    // Check if 1 is more then half of current examples
    return make_shared<TreeNode>(ans_1 < size / 2 ? 0 : 1);
  }

  // Make a decision tree node
 // cout << "Attr: " << attr << ", Split point: " << split_point << ", Child info: " << child_info << endl;
  //cout << "Left data size: " << split_it - begin_it << ", Right data size: " << end_it - split_it << endl;

  node = make_shared<TreeNode>(attr, split_point);
  if(split_point)
  {
      node->noofoneclass = int(split_it - begin_it);
      node->noofzeroclass = int(end_it - split_it);
  }
  else{
      node->noofzeroclass = int(split_it - begin_it);
      node->noofoneclass = int(end_it - split_it);
  }
  max_depth = max(max_depth,depth+1);
  node->left_child = gen_tree(node, begin_it, split_it, used_attr,depth + 1);
  node->right_child = gen_tree(node, split_it, end_it, used_attr,depth + 1);
  return node;
}

tuple<int, int, double> DecisionTree::choose_attr(
    const vector<vector<int>>::iterator &begin_it,
    const vector<vector<int>>::iterator &end_it,
    vector<vector<int>>::iterator &min_split_it,
            vector<bool> &used_attr,
    int skip_attr) {
  double min_info = MAX_INFO;
  int attr = -1, split_point = -1;
  vector<vector<int>>::iterator split_it;
  for(int i = 0; i < ATTR_SIZE; ++i) {
    // Don't choose the same attr which parent used
    // Or we'll get a pointless split point
    if(used_attr[i]) {
      continue;
    }

    // Use set, so we don't need to manually remove duplicate attr
    // We don't need ordered
    unordered_set<int> attr_vals;
    int val = 0;
    for(auto it = begin_it; it != end_it; ++it) {
      // Add all value into a set
      attr_vals.insert((*it)[i]);
        if(attr_vals.size() ==2)
            break;
    }

    double info;
    // Try to find a value to split example into two part
    /*for(int val : attr_vals) {
      split_it = partition(begin_it, end_it, [i, val](const auto& examples){
        return split_examples(i, val, examples);
      });
      // Calculate entropy
      info += entropy(begin_it, split_it, end_it);
    }*/
      if(attr_vals.size() ==2)
          val = 0;
      else{
          unordered_set<int>::iterator itr =  attr_vals.begin();
          val = (int) *itr;
      }
      split_it = partition(begin_it, end_it, [i, val](const auto& examples){
          return split_examples(i, val, examples);
      });
     
      // Calculate entropy
      if(bvariance)
          info = variance(begin_it, split_it, end_it);
      else
          info = entropy(begin_it, split_it, end_it);
      
    // If get a smaller entropy
    if(min_info > info) {
          min_info = info;
          attr = i;
          split_point = val;
          //min_split_it = split_it; // Write split point to #gen_tree
    }
  }
  if(attr != -1)
  {
      used_attr[attr] = true;
      min_split_it = partition(begin_it, end_it, [attr, split_point](const auto& examples){
            return split_examples(attr, split_point, examples);
        });
  }
  return make_tuple(attr, split_point, min_info);
}

// For multi set example
double DecisionTree::entropy(const array<vector<vector<int>>, 2> &examples_set) {
  double sum = 0;
  int total_size = int(examples_set[0].size() + examples_set[1].size());
  for(auto examples : examples_set) {
    sum += entropy(examples) * examples.size() / total_size;
  }
  return sum;
}

double DecisionTree::entropy(
    const vector<vector<int>>::iterator &begin_it,
    const vector<vector<int>>::iterator &split_it,
    const vector<vector<int>>::iterator &end_it
    ) {
  int total_size = int(end_it - begin_it);
  double info = entropy(begin_it, split_it) * (split_it - begin_it) / total_size;
  info += entropy(split_it, end_it) * (end_it - split_it) / total_size;
  return info;
}

// For single set example
double DecisionTree::entropy(vector<vector<int>> &examples) {
  double ans_count[2] = {0, 0};
  int size = (int)examples.size();
  for(auto example : examples) {
    ++ans_count[example[ATTR_SIZE]]; //24
  }
    if(ans_count[0] != 0 && ans_count[1] != 0)
    {
        return -((ans_count[0] / size) * log2(ans_count[0] / size))
        - ((ans_count[1] / size) * log2(ans_count[1] / size));
    }
    else if(ans_count[0] == 0 && ans_count[1] != 0){
        return - ((ans_count[1] / size) * log2(ans_count[1] / size));
    }
    else if(ans_count[0] != 0 && ans_count[1] == 0){
        return - ((ans_count[0] / size) * log2(ans_count[0] / size));
    }
    else{
        return 0;
    }
   // return -((ans_count[0] / size) * log2(ans_count[0] / size))
   // - ((ans_count[1] / size) * log2(ans_count[1] / size));
}

double DecisionTree::entropy(const vector<vector<int>>::iterator &begin_it, const vector<vector<int>>::iterator &end_it) {
  double ans_count[2] = {0, 0};
  int size = int(end_it - begin_it);
  for(auto it = begin_it; it != end_it; ++it) {
    ++ans_count[(*it)[ATTR_SIZE]]; //24
  }
    if(ans_count[0] != 0 && ans_count[1] != 0)
    {
        return -((ans_count[0] / size) * log2(ans_count[0] / size))
        - ((ans_count[1] / size) * log2(ans_count[1] / size));
    }
    else if(ans_count[0] == 0 && ans_count[1] != 0){
        return - ((ans_count[1] / size) * log2(ans_count[1] / size));
    }
    else if(ans_count[0] != 0 && ans_count[1] == 0){
        return - ((ans_count[0] / size) * log2(ans_count[0] / size));
    }
    else{
        return 0;
    }
   // return -((ans_count[0] / size) * log2(ans_count[0] / size))
   // - ((ans_count[1] / size) * log2(ans_count[1] / size));
}


// For multi set example
double DecisionTree::variance(const array<vector<vector<int>>, 2> &examples_set) {
    double sum = 0;
    int total_size = int(examples_set[0].size() + examples_set[1].size());
    for(auto examples : examples_set) {
        sum += variance(examples) * examples.size() / total_size;
    }
    return sum;
}

double DecisionTree::variance(
                             const vector<vector<int>>::iterator &begin_it,
                             const vector<vector<int>>::iterator &split_it,
                             const vector<vector<int>>::iterator &end_it
                             ) {
    int total_size = int(end_it - begin_it);
    double info = variance(begin_it, split_it) * (split_it - begin_it) / total_size;
    info += variance(split_it, end_it) * (end_it - split_it) / total_size;
    return info;
}

// For single set example
double DecisionTree::variance(vector<vector<int>> &examples) {
    double ans_count[2] = {0, 0};
    int size = (int)examples.size();
    for(auto example : examples) {
        ++ans_count[example[ATTR_SIZE]]; //24
    }
    return (ans_count[0] / size) * (ans_count[1] / size);
}

double DecisionTree::variance(const vector<vector<int>>::iterator &begin_it, const vector<vector<int>>::iterator &end_it) {
    double ans_count[2] = {0, 0};
    int size = int(end_it - begin_it);
    for(auto it = begin_it; it != end_it; ++it) {
        ++ans_count[(*it)[ATTR_SIZE]]; //24
    }
    return (ans_count[0] / size) * (ans_count[1] / size);
}


namespace {
  bool split_examples(int attr, int split, const vector<int> &example) {
    return example[attr] == split;
  }
}

std::shared_ptr<TreeNode> DecisionTree::reduced_error_prune(
                                              std::shared_ptr<TreeNode> parent,
                                              std::vector<std::vector<int>> &examples
                                              )
{
    isbest = false;
    if(!parent)
        return nullptr;
    //validation_accuracy = calc_accuracy(root,examples);
    while(parent->type !=0)
    {
        prune_node(parent,examples);
        if(isbest)
        {
            temp->type = 0;
            reduced_error_prune(parent,examples);
        }
        else
            return parent;
    }
    return parent;
}

void DecisionTree::prune_node(std::shared_ptr<TreeNode> parent,
                              std::vector<std::vector<int>> &examples)
{
    double accuracy;
    if(!parent)
        return;
    parent->type = 0;
    parent->val = parent->noofzeroclass/parent->noofoneclass > 1 ? 0:1;
    accuracy = calc_accuracy(root,examples);
    if(accuracy > validation_accuracy)
    {
        validation_accuracy = accuracy;
        isbest = true;
        temp = parent;
    }
    parent->type = 1;
    if(parent->left_child && parent->left_child->type == 1)
        prune_node(parent->left_child,examples);
    if(parent->right_child && parent->right_child->type == 1)
        prune_node(parent->right_child, examples);
}

double DecisionTree::calc_accuracy(std::shared_ptr<TreeNode> parent,
                                   std::vector<std::vector<int>> &examples)
{
    int correctclasification = 0, incorrectclasification = 0;
    std::vector<std::vector<int>>::iterator itr = examples.begin();
    int expected, actual;
    for (; itr != examples.end(); itr++)
    {
        actual = (*itr)[ATTR_SIZE];
        expected = predict_class(root,itr);
        if(actual == expected)
            correctclasification++;
        else
            incorrectclasification++;
    }
    return (double)correctclasification/(correctclasification+incorrectclasification);
}

int DecisionTree::predict_class(std::shared_ptr<TreeNode> parent,
                                std::vector<std::vector<int>>::iterator &itr)
{
    if(!parent)
        return 1;
    else if(parent->type == 0)
        return parent->val;
    int attr_val = (*itr)[parent->attr];
    if(attr_val == parent->split_point)
        return predict_class(parent->left_child,itr);
    else
        return predict_class(parent->right_child,itr);
    return 1;
}

int DecisionTree::depth_based_prune(std::shared_ptr<TreeNode> parent, std::vector<std::vector<int>> &examples)
{
    int depth_array[] = {5,10,15,20,50,100};
    double accuracy=0;
    int optimaldepth = max_depth;
    
    if (!parent)
        return 0;
    
        for(int i=0;i<6;i++)
        {
            if(max_depth > depth_array[i])
            {
                depth_prune(parent,depth_array[i]-1);
                accuracy = calc_accuracy(parent,examples);
                if(accuracy >= validation_accuracy)
                {
                    validation_accuracy = accuracy;
                    optimaldepth = depth_array[i];
                }
                undo_depth_prune(parent, depth_array[i]-1);
                accuracy = calc_accuracy(parent,examples);
                if((accuracy-validation_accuracy) > 0.00001)
                    cout << "error in undo depth based pruning"<< endl;
            }
        }
    if(max_depth != optimaldepth)
        depth_prune(parent,optimaldepth-1);
    return optimaldepth;
}

void DecisionTree::depth_prune(std::shared_ptr<TreeNode> parent, int depth)
{
    if(depth == 0)
    {
        if(parent->type == 1 && (parent->left_child->type != 0 || parent->right_child->type != 0 ) )
        {
            parent->type = 0;
           // cout << "attributes depth pruned:" << parent->attr << endl;
            parent->val = parent->noofzeroclass/parent->noofoneclass > 1 ? 0:1;
            return;
        }
        else
            return;
    }
    if(parent->left_child)
        depth_prune(parent->left_child, depth-1);
    if(parent->right_child)
        depth_prune(parent->right_child, depth-1);
    return;
}

void DecisionTree::undo_depth_prune(std::shared_ptr<TreeNode> parent, int depth)
{
    if(depth == 0)
    {
        if(parent->type == 0 && (parent->left_child != NULL || parent->right_child !=NULL))
        {
            parent->type = 1;
           // cout << "attributes undo depth pruned:" << parent->attr<< endl;
            return;
        }
        else
            return;
    }
    if(parent->left_child)
        undo_depth_prune(parent->left_child, depth-1);
    if(parent->right_child)
        undo_depth_prune(parent->right_child, depth-1);
    return;
}


