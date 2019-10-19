#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

#include "csv.hpp"
#include "decision_tree.hpp"
#include "tree_node.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  //ofstream fout("tree.dump");
  DecisionTree decision_tree;
  shared_ptr<TreeNode> root;
    
  if(argc == 5)
  {
      vector<vector<int>> training_data = read_csv(argv[2]);
      if(training_data.size() <= 0)
          return 0;
      vector<vector<int>> test_data = read_csv(argv[4]);
      if(test_data.size() <=0)
          cout << "There is no test data"<< endl;
      
      if(!strcmp(argv[1],"-e"))
      {
          decision_tree.bvariance = false;
          root = decision_tree.gen_tree(training_data);
          decision_tree.root = root;
          cout << "Entrophy Training data accuracy:"<<decision_tree.calc_accuracy(root,training_data)<< endl;
          if(test_data.size() > 0)
              cout << "Entrophy Test data accuracy:"<<decision_tree.calc_accuracy(root,test_data)<< endl;
      }
      else if(!strcmp(argv[1],"-v"))
      {
          decision_tree.bvariance = true;
          root = decision_tree.gen_tree(training_data);
          decision_tree.root = root;
          cout << "Variance Training data accuracy:"<<decision_tree.calc_accuracy(root,training_data)<< endl;
          if(test_data.size() > 0)
              cout << "Variance Test data accuracy:"<<decision_tree.calc_accuracy(root,test_data)<< endl;
      }
      else
      {
          vector<vector<int>> validation_data = read_csv(argv[3]);
          if(validation_data.size() <=0 )
          {
              cout << "There is no validation data for pruning"<<endl;
              return 0;
          }
          if(!strcmp(argv[1],"-er"))
          {
              decision_tree.bvariance = false;
              root = decision_tree.gen_tree(training_data);
              decision_tree.root = root;
              cout << "Training accuracy:"<<decision_tree.calc_accuracy(root,training_data)<< endl;
              decision_tree.validation_accuracy = decision_tree.calc_accuracy(root,validation_data);
              cout << "Validation accuracy before reduced error pruning:"<<decision_tree.validation_accuracy<< endl;
              root = decision_tree.reduced_error_prune(root,validation_data);
              //decision_tree.validation_accuracy = decision_tree.calc_accuracy(root,validation_data);
              cout << "Validation accuracy after reduced error pruning:"<< decision_tree.validation_accuracy<< endl;
              if(test_data.size() > 0)
                  cout << "Test accuracy:"<<decision_tree.calc_accuracy(root,test_data)<< endl;
          }
          else if(!strcmp(argv[1],"-ed"))
          {
              decision_tree.bvariance = false;
              root = decision_tree.gen_tree(training_data);
              decision_tree.root = root;
              cout << "Training accuracy:"<<decision_tree.calc_accuracy(root,training_data)<< endl;
              decision_tree.validation_accuracy = decision_tree.calc_accuracy(root,validation_data);
              cout << "Validation accuracy before depth based pruning:"<<decision_tree.validation_accuracy<< endl;
              cout << "depth before pruning:" << decision_tree.max_depth<< endl;
              cout << "depth after pruning:"<< decision_tree.depth_based_prune(root,validation_data)<< endl;
              //decision_tree.validation_accuracy = decision_tree.calc_accuracy(root,validation_data);
              cout << "Validation accuracy after depth based pruning:"<<decision_tree.validation_accuracy<< endl;
              if(test_data.size() > 0)
                  cout << "Test accuracy:"<<decision_tree.calc_accuracy(root,test_data)<< endl;
          }
          else if(!strcmp(argv[1],"-vr"))
          {
              decision_tree.bvariance = true;
              root = decision_tree.gen_tree(training_data);
              decision_tree.root = root;
              cout << "Training accuracy:"<<decision_tree.calc_accuracy(root,training_data)<< endl;
              decision_tree.validation_accuracy = decision_tree.calc_accuracy(root,validation_data);
              cout << "Validation accuracy before reduced error pruning:"<<decision_tree.validation_accuracy<< endl;
              root = decision_tree.reduced_error_prune(root,validation_data);
              decision_tree.validation_accuracy = decision_tree.calc_accuracy(root,validation_data);
              cout << "Validation accuracy after reduced pruning:"<<decision_tree.validation_accuracy<< endl;
              if(test_data.size() > 0)
                  cout << "Test accuracy:"<<decision_tree.calc_accuracy(root,test_data)<< endl;
          }
          else if(!strcmp(argv[1],"-vd"))
          {
              decision_tree.bvariance = true;
              root = decision_tree.gen_tree(training_data);
              decision_tree.root = root;
              cout << "Training accuracy:"<<decision_tree.calc_accuracy(root,training_data)<< endl;
              decision_tree.validation_accuracy = decision_tree.calc_accuracy(root,validation_data);
              cout << "Validation accuracy before depth based pruning:"<<decision_tree.validation_accuracy<< endl;
              cout << "depth before pruning:" << decision_tree.max_depth<< endl;
              cout << "depth after pruning:"<< decision_tree.depth_based_prune(root,validation_data)<< endl;
              decision_tree.validation_accuracy = decision_tree.calc_accuracy(root,validation_data);
              cout << "Validation accuracy after depth based pruning:"<<decision_tree.validation_accuracy<< endl;
              if(test_data.size() > 0)
                  cout << "Test accuracy:"<<decision_tree.calc_accuracy(root,test_data)<< endl;
          }
          else
          {
              cout <<"The given option is incorrect. Below are supported options:\n"
              <<"-e Entrophy\n" <<"-er Entropy with reduced error Pruning\n"
              <<"-ed Entropy with depth-based pruning\n"<<"-v Variance\n"
              <<"-vr Variance with reduced error pruning\n"
              <<"-vd Variance with depth-based pruning\n";
          }
      }
  }
  else{
      cout <<"Invalid number of command line arguments\n"
           <<"Expected format ./decisiontree.out -options trainingdata validationdata testdata";
  }

    /*vector<vector<int>> training_data = read_csv("train_c500_d100.csv");
    decision_tree.bvariance = true;
    root = decision_tree.gen_tree(training_data);
    decision_tree.root = root;
    cout << "Training accuracy:"<<decision_tree.calc_accuracy(root,training_data)<< endl;
    vector<vector<int>> validation_data = read_csv("valid_c500_d100.csv");
    decision_tree.validation_accuracy = decision_tree.calc_accuracy(root,validation_data);
    cout << "Validation accuracy before pruning:"<<decision_tree.calc_accuracy(root,validation_data)<< endl;
    root = decision_tree.reduced_error_prune(root,validation_data);
    cout << "Validation accuracy after pruning:"<<decision_tree.calc_accuracy(root,validation_data)<< endl;
    vector<vector<int>> test_data = read_csv("test_c300_d1000.csv");
    cout << "Test accuracy:"<<decision_tree.calc_accuracy(root,test_data)<< endl;
    
    cout << "depth before pruning:" << decision_tree.max_depth<< endl;
    int depth = decision_tree.depth_based_prune(root,validation_data);
    cout << "depth after pruning:"<< depth<< endl;
    //vector<vector<int>> test_data = read_csv("test_c500_d100.csv");
    cout << "Test accuracy:"<<decision_tree.calc_accuracy(root,test_data)<< endl;*/
    
    
 // root->serialize(fout);
  //fout.close();
  return 0;
}
