/**
 * Kostra hlavickoveho souboru 3. projekt IZP 2018/19
 * a pro dokumentaci Javadoc.
 * @name Project 3 - Simple Cluster Analysis
 * @mainpage Description
 * Program is used to create the N clusters out of objectcs from input file using the cluster analysis theory.
 * @section
 * Cluster analysis or clustering is the task of grouping a set of objects in such a way that objects in the same group (called a cluster) are more similar (in some sense) to each other than to those in other groups (clusters).
 * @file proj3.h
 * @brief Header file for the main file of the project.
 * @author Michal Koval <xkoval17@stud.fit.vutbr.cz>
 * @date 07/12/2018
 * @version 1.0
 * @see https://en.wikipedia.org/wiki/Cluster_analysis
 */

 /**
 * @brief Structure containing identifier and x y coordinates of object in 2D plane.
 */
struct obj_t {
    int id;
    float x;
    float y;
};

 /**
 * @brief Structure containing the current size of array, capacity of array and the array of objects.
 */
struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/**
 * @defgroup cluster Functions working with clusters
 * @{
 */

/**
 * Inicialization of cluster. Alocates enough memory for 'cap' objects in the clusters.
 *
 * @param[in,out] c Cluster which is to be inicialized.
 * @param[in]  cap Required capacity of cluster.
 * @pre Required capacity must be greater or equal to 0
 * @pre Pointer to cluster must not point to NULL
 * @post Cluster 'c' will be inicialized, if no alocation error occured.
 */
void init_cluster(struct cluster_t *c, int cap);
/**
 * Removes all objects from cluster and inicializes to empty cluster.
 *
 * @param[in,out] c Cluster which is to be emptied
 * @param[in,out] cap Capacity of cluster c
 * @post Memory for objects of cluster 'c' will be deallocated and size of cluster set to 0
 */
void clear_cluster(struct cluster_t *c);

/**
 * Chunk of data for cluster reallocation
 */
extern const int CLUSTER_CHUNK;

/**
 * Resizes cluster to new capacity.
 *
 * @param[in] c Cluster which is to be resized
 * @param[in] new_cap Required capacity of cluster
 * @pre Cluster 'c' must be already allocated
 * @pre Capacity of 'c' has to be bigger or equal to 0
 * @pre New capacity has to be bigger or equal to 0
 * @post Capacity of cluster 'c' will be resized to 'new_cap' if no error occured
 * @return resized cluster or NULL in case of error
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * Adds object to the end of the cluster.
 * Resizes the cluster if its capacity is full.
 *
 * @param[in,out] c Destination cluster
 * @param[in] obj Object which is to be appended
 * @post Object 'obj' will be added to the last position of cluster 'c', if no error has occured
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * Prints cluster on standard output
 *
 * @param[in] c Cluster which is to be printed.
 * @post Prints objects of cluster 'c' on standard output
 */
void print_cluster(struct cluster_t *c);

/**
 * Sorts cluster
 *
 * @param[in,out] c Cluster which is to be sorted.
 * @post Cluster 'c' will be sorted according the identifiers of objects in cluster.
 */
void sort_cluster(struct cluster_t *c);

/**
 * @}
 */

/**
 * @defgroup array Functions working with array of clusters
 * @{
 */

/**
 * Merges 2 clusters into 1 cluster.
 *
 * @param[in,out] c1 Destination cluster
 * @param[in] c2 Target cluster
 * @pre Both clusters must be already inicialized
 * @post Objects of cluster c2 will be added to cluster c1. Cluster c1 will be resized if needed. Objects in resulting cluster c1 will be sorted.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * Removes cluster on index 'idx' from array 'carr'
 *
 * @param[in,out] carr Array of clusters
 * @param[in,out] narr Number of cluster in array
 * @param[in,out] idx Index of target cluster
 * @pre Number of objects 'narr' in cluster must be greater than 0
 * @pre Index 'idx' of target object must be smaller than number of objects in cluster 'narr'
 * @return new number of clusters in array
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * Calculates the Euclidean distance between 2 objects.
 * @see https://en.wikipedia.org/wiki/Euclidean_distance
 * @param[in] o1 Object 1
 * @param[in] o2 Object 2
 * @pre Both objects must be inicialized
 * @return Distance between the objects 'o1', 'o2'
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * Calculates the distance between two clusters. The distance of two clusters is equal to the lowest distance among all of the objects of said clusters.
 * @param[in] c1 Cluster 1
 * @param[in] c2 Cluster 2
 * @pre Both cluster must be inicialized
 * @pre Size of both clusters must be greater than 0
 * @return Distance between the clusters 'c1', 'c2'
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * Finds the neighbouting clusters in the array of clusters. The two clusters with lowest distance among all.
 *
 * @param[in] carr Array of clusters
 * @param[in] narr Number of clusters in array
 * @param[in] c1 Index of cluster 1 in array carr
 * @param[in] c2 Index of cluster 2 in array carr
 * @pre Number of clusters in array 'narr' must be greater than 0
 * @post Indexes of two closest clusters will be saved in 'c1' and 'c2'
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * Loads the objects from file and saves them all separately in their own clusters in the array of clusters.
 *
 * @param[in] filename Name of file in which objects are stored
 * @param[in,out] arr Array of clusters
 * @pre File 'filename' will exist
 * @pre File 'filename' will be in correct format
 * @post Objects from file under name 'filename' will be loaded and saved in clusters in the array of clusters 'arr', if no error has occured
 * @return number of loaded objects, in case of error 0
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * Prints array of clusters on stdout.
 * Number of clusters in array is required.
 * @param[in] carr Array of clusters
 * @param[in] narr Numbers of clusters in array of clusters
 * @post All the clusters in the arrray of clusters 'carr' will be printed on standard output
 */
void print_clusters(struct cluster_t *carr, int narr);

/**
 * @}
 */
