This repositry has following algorithms implemented in c++

– Naive Decision tree learner with Entropy as the impurity heuristic
– Naive Decision tree learner with Variance as the impurity heuristic.
– Decision tree learner with Entropy as the impurity heuristic and reduced error pruning
– Decision tree learner with Variance as the impurity heuristic and reduced error pruning
– Decision tree learner with Entropy as the impurity heuristic and depth-based pruning
– Decision tree learner with Variance as the impurity heuristic and depth-based pruning
– Random Forests


Variance impurity heuristic described below.
Let K denote the number of examples in the training set. Let K0 denote the number of training examples that have class = 0 and K1 denote the number of training examples that have class = 1. The variance impurity of the training set S is defined as:
VI(S) = K0*K1/K*K
Notice that the impurity is 0 when the data is pure. The gain for this impurity is defined as usual.
Gain(S, X) = VI(S) − Σx∈Values(X) Pr(x)VI(Sx)
where X is an attribute, Sx denotes the set of training examples that have X = x and Pr(x) is the fraction of the training examples that have X = x (i.e., the number of training examples that have X = x divided by the number of training examples in S).

Depth-based pruning uses maximum depth dmax as a hyper-parameter, namely in the decision tree prune all nodes having depth larger than dmax. We will assume that dmax takes values from the following set: {5,10,15,20,50,100}. We tune the hyper- parameters using the validation set.
