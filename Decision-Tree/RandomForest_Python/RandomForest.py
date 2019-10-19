import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score
import sys

def random_forest_classifier(features, target):
    """
    To train the random forest classifier with features and target data
    :param features:
    :param target:
    :return: trained random forest classifier
    """
    clf = RandomForestClassifier()
    clf.fit(features, target)
    return clf

def main():
    """
    Main function
    :return:
    """
    # Load the csv file into pandas dataframe
    training_data = pd.read_csv(sys.argv[1], header=None)
    vaildation_data = pd.read_csv(sys.argv[2], header = None)
    test_data = pd.read_csv(sys.argv[3], header = None)
    
    new_data = pd.concat([training_data, vaildation_data])
    
    Xtst = test_data.iloc[:, 0:500]
    ytst = test_data.iloc[:,-1]
    
    Xtra = new_data.iloc[:, 0:500]
    ytra = new_data.iloc[:,-1]
    
    # Create random forest classifier instance
    trained_model = random_forest_classifier(Xtra, ytra)
    predictions = trained_model.predict(Xtst)
    
    
    # Train and Test Accuracy
    print ("Train Accuracy :: ", accuracy_score(ytra, trained_model.predict(Xtra)))
    print ("Test Accuracy  :: ", accuracy_score(ytst, predictions))

if __name__ == "__main__":
    main()