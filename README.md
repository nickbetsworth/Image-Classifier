# Image-Classifier
Clusters and positions databases of images with respect to their visual similarity.

### Features
* Clustering of large database of images (**K-means** & **GMM** supported)
* Classification of new images with **Random Forest** classifier
* Positioning of images within each cluster via layout algorithms (tree layout)
* Bag-of-words model for quick comparison of local features

#### Available Image Features:
* Colour Histograms
* Local Features supported by OpenCV (SIFT, SURF, etc..)

#### Dependencies
* OpenCV 2.4.9+
* Qt 5.3
* Open Graph Drawing Framework

#### Screenshots
##### Histogram clustering
<img src="https://i.imgur.com/NiazLOQ.jpg" width="800px" />

##### Browsing inside a cluster with local descriptor (SURF) clustering
Note the various different types of category of motorcycle are grouped together.

<img src="https://i.imgur.com/nsX9Plv.jpg" width="800px" />
