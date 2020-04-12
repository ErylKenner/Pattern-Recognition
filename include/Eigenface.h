#ifndef EIGENFACE_H_
#define EIGENFACE_H_

#include <string>
#include <vector>
#include "Eigen/Dense"
#include "Image.h"

using namespace Eigen;

// N^2 - number of pixels in an image
// M - number of images in the training set
// K - number of images in the testing set

class Eigenface
{
    public:
        // (N^2)-vector
        VectorXf averageFace;

        // (N^2 x M)-matrix with eigenvectors/faces are the columns
        MatrixXf eigenfaces;

        // (M)-vector of eigenvalues that correspond to the eigenfaces
        VectorXf eigenvalues;

        // (M x M)-matrix with columns of the eigenspace representation of each training image
        MatrixXf eigenspaceTrainingValues;

        std::vector<std::string> imageNames;

        // Image information used for outputting images
        int imageRows, imageCols, imageRange;

        // Initializes the averageFace, eigenfaces, eigenvalues, eigenspaceTrainingValues
        Eigenface(std::string trainingDirectory);

        // testingImages - (N^2 x K)-matrix of testing images
        // infoRatio - percent information perserved / percent eigenvalues used
        // Returns the image index of the closest topMatches matches for each column of testingImages
        std::vector<std::vector<int>> GetClosestMatches(const MatrixXf &testingImages, int topMatches, float infoRatio) const;

        // testingImages - (N^2 x K)-matrix of testing images
        // infoRatio - percent information perserved / percent eigenvalues used
        // Returns the error/distance between the reconstructed and normalized image for each column of testingImages
        std::vector<float> GetDetectionError(const MatrixXf &testingImages, float infoRatio) const;

        // Returns the averageFace in image format
        Image<GreyScale> GetAverageImage() const;

        // Returns the eigenfaces/vectors in image format on range [start, end)
        std::vector<Image<GreyScale>> GetEigenfaceImages(int start, int end) const;
    

    private:
        // Performs the work of the constructor
        MatrixXf GetTrainingData(std::string trainingDirectory);

        // Tranforms the vector to be in range [0, range]
        VectorXf AdjustToImageRange(const VectorXf &image, int range) const;

        // Returns the (N^2)-vector of the average of the colums of (N^2 x M)-matrix images
        VectorXf GetAverageFace(const MatrixXf &images) const;

        // normalizedImages - (N^2 x M)-matrix aka A in which the columns are the normalized images
        // Sets the eigenvalues and eigenfaces members
        // Computes A'A and uses this to find the eigenvalues/vectors of AA'
        void SetEigenvaluesEigenvectors(const MatrixXf &normalizedImages);

        // images - (N^2 x M, K, 1)-matrix
        // Returns an (M x M, K, 1)-matrix in which the columns are the eigenspace representation of the columns of images
        MatrixXf ComputeEigenSpaceValues(const MatrixXf &images) const;

        // images - (N^2 x M, K, 1)-matrix
        // Subtracts the average face from each column of images and returns the resulting value
        MatrixXf NormalizeImages(const MatrixXf &images) const;

        // eigenspaceValues - (M x M, K, 1)-matrix
        // eigenCount - number of eigenvectors used in the reconstruction
            // Must be less than or equal to M
        // Reconstructs the columns of eigenspaceValues using the first eigenCount eigenfaces
        MatrixXf ReconstructImages(const MatrixXf &eigenspaceValues, int eigenCount) const;

        // Outputs the averageFace, eigenFaces, and the eigenspaceTrainingValues to a file
        void OutputTrainingData(std::string outputFileName) const;

        // Returns the MahalanobisDistance between the eigenspace representations of the two images
        // eigenspaceImage - (M)-vector
        // eigenCount - number of eigenvectors considered in the calculation
            // Must be less than or equal to M
        float MahalanobisDistance(const VectorXf &eigenspaceImage1, const VectorXf &eigenspaceImage2, int eigenCount) const;

        // Returns the EuclideanDistance between the eigenspace representations of the two images
        // eigenspaceImage - (M)-vector
        // eigenCount - number of eigenvectors considered in the calculation
            // Must be less than or equal to M
        float EuclideanDistance(const VectorXf &eigenspaceImage1, const VectorXf &eigenspaceImage2, int eigenCount) const;

        // Compares two image names (from Faces_FA_FB naming convention)
        // Returns true if the images are of the same person, false if not
        bool ImageNamesEqual(std::string name1, std::string name2) const;
};


#endif