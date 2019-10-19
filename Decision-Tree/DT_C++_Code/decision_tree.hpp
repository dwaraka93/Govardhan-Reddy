#ifndef DECISION_TREE_HPP_INCLUDE
#define DECISION_TREE_HPP_INCLUDE

#include <array>
#include <vector>
#include <tuple>
#include <memory>
#include <cstddef>

struct TreeNode;

class DecisionTree {
public:
  DecisionTree() {}
    std::shared_ptr<TreeNode> root;
    std::shared_ptr<TreeNode> temp;
    bool  isbest = false;
  std::shared_ptr<TreeNode> gen_tree(std::vector<std::vector<int>> &examples);
  std::shared_ptr<TreeNode> gen_tree(
    std::shared_ptr<TreeNode> parent,
    const std::vector<std::vector<int>>::iterator &begin_it,
    const std::vector<std::vector<int>>::iterator &end_it,
    const std::vector<bool> attr,
    int depth = 0
  );
    int  max_depth = 0;
    bool bvariance = false;
    double validation_accuracy = 0;
    std::shared_ptr<TreeNode> reduced_error_prune(
                                       std::shared_ptr<TreeNode> parent,
                                       std::vector<std::vector<int>> &examples
                                       );
    double calc_accuracy(std::shared_ptr<TreeNode> parent,
                         std::vector<std::vector<int>> &examples);
    
    int depth_based_prune(std::shared_ptr<TreeNode> parent, std::vector<std::vector<int>> &examples);
    
private:
  //std::size_t examples_size;
    
  double entropy(const std::array<std::vector<std::vector<int>>, 2> &example_set);

  double entropy(
    const std::vector<std::vector<int>>::iterator &begin_it,
    const std::vector<std::vector<int>>::iterator &split_it,
    const std::vector<std::vector<int>>::iterator &end_it
  );

  double entropy(std::vector<std::vector<int>> &examples);
  double entropy(
    const std::vector<std::vector<int>>::iterator &begin_it,
    const std::vector<std::vector<int>>::iterator &end_it
  );
    
    double variance(const std::array<std::vector<std::vector<int>>, 2> &example_set);
    
    double variance(
                   const std::vector<std::vector<int>>::iterator &begin_it,
                   const std::vector<std::vector<int>>::iterator &split_it,
                   const std::vector<std::vector<int>>::iterator &end_it
                   );
    
    double variance(std::vector<std::vector<int>> &examples);
    double variance(
                   const std::vector<std::vector<int>>::iterator &begin_it,
                   const std::vector<std::vector<int>>::iterator &end_it
                   );

  std::tuple<int, int, double> choose_attr(
    const std::vector<std::vector<int>>::iterator &begin_it,
    const std::vector<std::vector<int>>::iterator &end_it,
    std::vector<std::vector<int>>::iterator &split_it,
    std::vector<bool> &used_attr,
    int skip_attr = -1
  );
    
    int predict_class(std::shared_ptr<TreeNode> parent,
                      std::vector<std::vector<int>>::iterator &itr);
    
    void prune_node(std::shared_ptr<TreeNode> parent,
                    std::vector<std::vector<int>> &examples);
    
    void depth_prune(std::shared_ptr<TreeNode> parent, int depth);
    
    void undo_depth_prune(std::shared_ptr<TreeNode> parent, int depth);

};

#endif /* end of include guard: DECISION_TREE_HPP_INCLUDE */

