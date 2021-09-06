/**
 * CSC A48 - Intro to Computer Science II, Summer 2020
 * 
 * This is the program file where you will implement your solution for
 * assignment 2. Please make sure you read through this file carefully
 * and that you understand what is provided and what you need to complete.
 * 
 * You will need to have read the handout carefully. 
 * 
 * Parts where you have to implement functionality are clearly labeled TODO
 * 
 * Be sure to test your work thoroughly, our testing will be extensive
 * and will check that your solution is *correct*, not only that it
 * provides functionality.
 * 
 * (c) 2020 Mustafa Quraish 
 */

#include "imgUtils.c"

/**
 * This is the structure we are going to use to store each individual node of 
 * the BST. Remember that each Quad corresponds to a rectangular area on the 
 * image:
 *
 *                (tx,ty)         w
 *                   x-------------------------
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                 h |          Quad          |
 *                   |    key = tx+(ty*sx)    |
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                   -------------------------x
 *                                       (tx + w, ty + h)
 *
 */
typedef struct quad {
  int tx, ty;  // The (x,y) coordinates of the top-left pixel in the quad
  int w;       // How many pixels wide the quad is
  int h;       // How many pixels high the quad is

  int sx;  // Width of the original image, this is needed for the key.
           // This *MUST* be the same for all nodes in the BST

  int key;  // A unique identifier (remember we discussed BST nodes
            // should have unique keys to identify each node). The
            // key identifier here will be created as:
            //       key = tx + (ty * sx)
            // This means that only one quad can start at a specific
            // pixel.

  int wsplit;  // 1 if this quad is supposed to be split along the width
               // 0 if this quad is supposed to be split along the height

  /** 
   * TODO: Complete the definition of the Quad struct
   */

  struct quad* left;
  struct quad* right;

} Quad;

///////////////////////////////////////////////////////////////////////////////

Quad *new_Quad(int tx, int ty, int w, int h, int wsplit, int sx) {
  /**
   * This function creates and initializes a new Quad for a rectangle starting 
   * at (tx, ty) with a width 'w' and height 'h'. The width of the image in 
   * which this rectangle exists in is 'sx', use this to compute the key as:
   *
   * 		key = tx + (ty * sx)
   * 
   * TODO: Implement this function
   */

  //Make a new Quad
  Quad* new_quad = (Quad*)calloc(1, sizeof(Quad));

  //Initialize values for this new Quad
  new_quad->tx = tx;
  new_quad->ty = ty;
  new_quad->w = w;
  new_quad->h = h;
  new_quad->wsplit = wsplit;
  new_quad->sx = sx;
  new_quad->key = tx + (ty * sx); 
  new_quad->left = NULL;
  new_quad->right = NULL;
  return new_quad;
}

///////////////////////////////////////////////////////////////////////////////

Quad *BST_insert(Quad *root, Quad *new_node) {
  /**
   * This function inserts a new Quad node into the BST rooted at 'root'. The 
   * new_node must already be initialized with valid data, and must have a 
   * unique key.
   *
   * Your function must make sure that there are no duplicate nodes with the 
   * same key in the BST, and if it finds any you should print the following 
   * message to the screen:
   *
   * printf("Duplicate Quad (tx,ty,sx)=%d,%d, %d, was ignored\n",....); 
   * (of course you need to provide the relevant variables to print)
   *
   * And it must return without inserting anything in the BST.
   *
   * TODO: Implement this function
   */

  //Case where root is NULL
  if(root == NULL){
    return new_node;
  }
  //Case where new_node's key is less than root
  if(new_node->key < root->key){
    root->left = BST_insert(root->left, new_node);
  }
  //Case where new_node's key is greater than root
  else if(new_node->key > root->key){
    root->right = BST_insert(root->right, new_node);
  }
  //Case where new_node's key is equal to root
  else{
    printf("Duplicate Quad (tx,ty,sx)=%d, %d, %d, was ignored\n", new_node->tx, new_node->ty, new_node->sx);
  }
  return root;
}

///////////////////////////////////////////////////////////////////////////////

Quad *BST_search(Quad *root, int tx, int ty) {
  /**
   * This function searches the BST for a Quad at the speficied position. If 
   * found, it must return a pointer to the quad that contains it.
   *
   * Search has to happen according to the BST search process - so you need to 
   * figure out what value to use during the search process to decide which 
   * branch of the tree to search next.
   *
   * Note that the 'sx' value does not need to be passed in here since it must 
   * be the same as the one in any Quad already in the tree.
   *
   * Return NULL if the Quad doesn't exist in the BST.
   *
   * TODO: Implement this function
   */
  
  //Case where root is NULL or temp_key is equivalent to root->key
  if(root == NULL){
    return NULL;
  }
  
  //Create a temporary key that utilizes the given tx and ty values
  int temp_key = tx + (ty * (root->sx));

  //Case where temp_key is less than root->key (lies in the left subtree)
  if(temp_key < root->key){
    return BST_search(root->left, tx, ty);
  }
  //Case where temp_key is greater than root->key (lies in the right subtree)
  else if(temp_key > root->key){
    return BST_search(root->right, tx, ty);
  }
  else{
    return root;
  }
}

///////////////////////////////////////////////////////////////////////////////

Quad *find_successor(Quad *right_child) {
  /**
   * This function finds the successor of a Quad node by searching the right 
   * subtree for the node that is most to the left (that will be the node
   * with the smallest key in that subtree)
   *
   * TODO: Implement this function
   */
  
  //Create a temporary Quad that stores right_child
  Quad* temp = right_child;
  
  //Loop to find left most node in the left subtree to right_child
  while(temp != NULL && temp->left != NULL){
    temp = temp->left;
  }
  return temp;
}

///////////////////////////////////////////////////////////////////////////////

Quad *BST_delete(Quad *root, int tx, int ty) {
  /**
   * Deletes from the BST a Quad at the specified position. You must implement 
   * the three cases of BST deletion we discussed in class. Make sure the 
   * function can remove a Quad at any position without breaking the tree!
   *
   * Once again, remember that 'sx' is stored in the tree.
   *
   * TODO: Implement this function
   */

  //Case where root is NULL a.k.a base case
  if(root == NULL){
    return NULL;
  }

  //Create a temporary key that utlizes the given tx and ty values
  int temp_key = tx + (ty * (root->sx));

  //Case where temp_key is equivalent to root->key (four subcases follow)
  if (temp_key == root->key){
    //Both Subcase 1 and Subcase 2 are used to solve Subcase 0 (Subcase where root is a leaf node)
    //Subcase 1: root has a right child
    if(root->left == NULL){
      Quad* temp_right = root->right;
      free(root);
      return temp_right;
    }
    //Subcase 2: root has a left child
    else if(root->right == NULL){
      Quad* temp_left = root->left;
      free(root);
      return temp_left;
    }
    //Subcase 3: root has two children 
    else{
      Quad* temp = find_successor(root->right);
      root->tx = temp->tx;
      root->ty = temp->ty;
      root->w = temp->w;
      root->h = temp->h;
      root->wsplit = temp->wsplit;
      root->key = temp->key;
      root->right = BST_delete(root->right, temp->tx, temp->ty);
    }
  }
  //Case where temp_key is less than root->key (lies in left subtree)
  else if(temp_key < root->key){
    root->left = BST_delete(root->left, tx, ty);
  }
  //Case where temp_key is greater than root->key (lies in the right subtree)
  else{
    root->right = BST_delete(root->right, tx, ty);
  }
  return root;
}

///////////////////////////////////////////////////////////////////////////////

Quad *delete_BST(Quad *root) {
  /**
   * This function deletes the BST and frees all memory used for nodes in it. 
   * Recall that there is a specific order in which this needs to be done! 
   * (consult the Unit 4 notes as needed)
   * 
   * This function should return NULL.
   *
   * TODO: Implement this function
   */

  //Best course of action to delete the entire tree would be postorder traversal
  if(root != NULL){
    delete_BST(root->left);
    delete_BST(root->right);
    free(root);
  }
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////

void BST_inorder(Quad *root, int depth) {
  /**
   * This function performs an in-order traversal of the BST and prints out the
   * information for each Quad using this exactly this print statement:
   *
   *  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",...)
   *
   * Obviously, you must provide the variables to the printf function - we're 
   * just giving you the formatting string.
   *
   * The depth value is increased by 1 for each recursive call so when you 
   * print, you can see at what level each node is located! (this should help 
   * you debug your code by making it easier to check the shape of your BST).
   *
   * TODO: Implement this function
   */

  if (root != NULL){
    BST_inorder(root->left, depth + 1);
    printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n", depth, root->key, root->tx, root->ty, root->w, root->h, root->wsplit);
    BST_inorder(root->right, depth + 1);
  }
  return;
}

///////////////////////////////////////////////////////////////////////////////

void BST_preorder(Quad *root, int depth) {
  /**
   * This function performs a pre-order traversal of the BST and prints out the 
   * information for each Quad using this exactly this print statement:
   *
   *  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",...)
   *
   * Obviously, you must provide the variables to the printf function - we're 
   * just giving you the formatting string.
   *
   * The depth value is increased by 1 for each recursive call so when you 
   * print, you can see at what level each node is located! (this should help 
   * you debug your code by making it easier to check the shape of your BST).
   *
   * TODO: Implement this function
   */

  if (root != NULL){
    printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n", depth, root->key, root->tx, root->ty, root->w, root->h, root->wsplit);
    BST_preorder(root->left, depth + 1);
    BST_preorder(root->right, depth + 1);
  }
  return;
}

///////////////////////////////////////////////////////////////////////////////

void BST_postorder(Quad *root, int depth) {
  /**
   * This function performs a post-order traversal of the BST and prints out 
   * the information for each Quad using this exactly this print statement:
   *
   *  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",...)
   *
   * Obviously, you must provide the variables to the printf function - we're 
   * just giving you the formatting string.
   *
   * The depth value is increased by 1 for each recursive call so when you 
   * print, you can see at what level each node is located! (this should help 
   * you debug your code by making it easier to check the shape of your BST).
   *
   * TODO: Implement this function
   */
  
  if (root != NULL){
    BST_postorder(root->left, depth + 1);   
    BST_postorder(root->right, depth + 1);
    printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n", depth, root->key, root->tx, root->ty, root->w, root->h, root->wsplit);
  }
  return;
}

///////////////////////////////////////////////////////////////////////////////
// NOTE: For the remaining functions, you may assume the following:          //
//       (1) All the Quads are valid (None of them go outside the image)     //
//       (2) They don't overlap  (A pixel will not be in multiple Quads)     //
///////////////////////////////////////////////////////////////////////////////

int get_colour(Image *im, Quad *q) {
  /**
   * Given an image 'im' and a Quad 'q', get the colour we should be assigning
   * to the pixels that are in it, and return it. For the sake of this 
   * assignment, we will say this is *average* colour of all the pixels in 
   * the quad.
   *
   * The pixel data is stored in a one dimensional array called 'data' in the
   * image struct. Make sure you look at the definition of this to understand
   * how the image is stored. Remember that the pixel data is stored in
   * row-major order, so to get the colour for pixel (x,y) you will look at the
   * index
   *
   *                           x + (y * sx)
   *
   * of the array.
   *
   * TODO: Implement this function. You should not be getting any values 
   *       outside the range of the pixels [0-255] if you have implemented 
   *       this correctly.
   */

  //Create a variable that stores all the colours at the pixels added
  int colour = 0;
  //Create a variable that stores the average colour among the pixels in the Quad
  int avg_colour = 0;
  //Create a variable that stores the index of a pixel
  int index = 0;
  //Create a counter to store the number of pixels in the Quad
  int num_pixels = 0;

  //Loop through the Quad to access each pixel and extract the colour at pixel 
  for(int i = 0; i < q->w; i++){
    for(int j = 0; j < q->h; j++){
      index = q->tx + i + ((q->ty + j) * im->sx);
      colour += im->data[index];
      num_pixels++;
    }
  }

  //Find average colour of pixels in the Quad
  avg_colour = colour / num_pixels;
  return avg_colour;
}

///////////////////////////////////////////////////////////////////////////////

int similar(Image *im, Quad *q, int threshold) {
  /**
   * Given an image 'im', check if the colours in the area corresponding to the 
   * Quad 'q' are all similar. If not, we will have to split it. For the 
   * purpose of this assigment, we say the colours in a Quad are similar if
   *
   *          maxCol - minCol <= threshold
   *
   * where maxCol and minCol are the maximum and minimum values respectively
   * of the pixel colours in the Quad. The threshold is a parameter. This
   * function should return a 0 if the pixels are not similar enough and the
   * Quad needs to be split, and 1 otherwise.
   *
   * TODO: Implement this function
   */
  
  //Create a variable to store the minimum pixel colour
  int minCol = 255;
  //Create a variable to store the maximum pixel colour
  int maxCol = 0;
  //Create a variable to store the index of a pixel
  int index = 0;

  //Loop through the pixels in the Quad and store minCol and maxCol 
  for(int i = 0; i < q->w; i++){
    for(int j = 0; j < q->h; j++){
      index = q->tx + i + ((q->ty + j) * im->sx);;
      //Check if im->data[index] is greater than minCol
      if(im->data[index] < minCol){
        minCol = im->data[index];
      } 
      else if(im->data[index] > maxCol){
        maxCol = im->data[index];
      }
    }
  }
   
  //Check if the colours in the pixels are similar 
  if(maxCol - minCol <= threshold){
    return 1;
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////

Quad *split_tree(Image *im, Quad *root, int threshold) {
  /**
   * This function traverses the BST, and for each existing Quad, checks if 
   * the pixels in the quad are of a similarcolour using the similar() function 
   * and the given threshold. If they are not, then the Quad needs to be split 
   * into 2 Quads(which will hopefully have pixels that are more similar to 
   * each other).
   *
   * To do this, first we need to decide in which direction we are going to 
   * split the Quad. For this, we will use the 'wsplit'field.
   *
   *    - If wsplit = 1, then we split it along the width (ie, we will now have 
   *                     2 quads with the same heightand half the width as the 
   *                     original one)
   *
   *    - If wsplit = 0, then we split along the height.
   *
   * NOTE: We don't always want to split the Quads in the same direction
   *       every time this function is called, because then we could just
   *       end up with very thin and long/tall quads, which wouldn't be very
   *       helpful to what we are trying to do. So, we need to make sure that
   *       once we split a Quad, that we invert the value of the 'wsplit'
   *       variable in both the new nodes, so they split the other way.
   *
   * -------------------------------------------------------------------------
   *
   * For example, if our Quad had the following values:
   *    (tx:ty = 0:0     w = 512,   h = 512,  wsplit = 1) ---> A
   *
   *                (0,0)
   *                   x-------------------------
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |           A            |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   -------------------------x
   *                                         (512, 512)        
   *                
   *                                     * this pixel is not IN the image, just
   *                                       represents the 'corner'. The bottom
   *                                       right pixel, as always, is (511,511)
   * 
   * it would be split along the width, and the resulting two Quads
   * we would get would be as follows:
   *
   *     (tx:ty =  0 :0     w = 256,   h = 512,  wsplit = 0) ---> B
   *     (tx:ty = 256:0     w = 256,   h = 512,  wsplit = 0) ---> C
   *
   *
   *                (0,0)       (256, 0)
   *                   x-----------x-------------
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   |     B     |      C     |
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   -------------------------x
   *                                         (512, 512)
   *
   *   - Note that we want to always split it in exactly half, but if the
   *     width/height is an odd number then round down.
   *
   *   - Further note that 'wsplit' on both of these has now been set to 0.
   *     If they were split again, the resulting Quads would have wsplit = 1.
   *
   * --------------------------------------------------------------------------
   *
   * Now, once you know how it needs to be split, carefully form these two
   * Quads, with the correct positions and sizes, and replace the the original 
   * one with them.
   *
   * This function is crunchy - and if you don't think it through before you 
   * start implementing it you'll run into all kinds of trouble.
   *
   * This is the problem solving exercise for A2, so don't look for people
   * on Piazza to give you answers, or tell you what to do, or verify you're
   * doing the right thing.
   *
   * It's up to you how to solve this, and if you want an opinion, you can
   * come to visit us during office hours! The included file `point.pgm` is
   * a good candidate image to test this function on.
   *
   * Expected result: The BST will have at most twice as many Quads
   *                  as before, depending on how many of them needed to be 
   *                  split.
   *
   * TODO: Implement this function
   */

  //Create a variable that stores similarity of colours of pixels in a Quad
  int similarity = 0;
  //Create a variable that stores the midpoint of the width or height for a Quad
  int midpoint = 0;
  //Create a variable to store the point at which the second_half should start
  int starting_point = 0;

  //Create a Quad for the second half when split by width
  Quad* second_half_by_w = NULL;
  //Create a Quad for the second half when split by height
  Quad* second_half_by_h = NULL;

  //Traverse through the BST using postorder traversal to stop duplicate splits
  if(root != NULL){
    split_tree(im, root->left, threshold);   
    split_tree(im, root->right, threshold);   
    similarity = similar(im, root, threshold);
    //Check that there is no similarity and the Quad will be split by width
    if(root->wsplit == 1 && similarity == 0){
      //Change the existing Quad's wsplit to be a split by height
      root->wsplit = 0;
      //Find the midpoint of the width and change the width to half it's original  
      midpoint = root->w / 2;
      root->w = midpoint;
   
      //Create the second half, but first find a new starting point for the root->tx value
      starting_point = root->tx + midpoint;
      second_half_by_w = new_Quad(starting_point, root->ty, midpoint, root->h, 0, im->sx);
      root = BST_insert(root, second_half_by_w);
    }
    //Check that there is no similarity and the Quad will be split by height
    else if(root->wsplit == 0 && similarity == 0){
      //Change the existing Quad's values to become the first half
      root->wsplit = 1;  
      //Find the midpoint of the height and change the height to half it's orignal
      midpoint = root->h / 2;
      root->h = midpoint;
      
      //Create the second half, but first find a new starting point for the root->ty value
      starting_point = root->ty + midpoint;
      second_half_by_h = new_Quad(root->tx, starting_point, root->w, midpoint, 1, im->sx);
      root = BST_insert(root, second_half_by_h);
    }
  }   
  return root;
}

///////////////////////////////////////////////////////////////////////////////

void drawOutline(Image *im, Quad *root, unsigned char col) {
  /**
   * Given an image 'im' and a BST rooted at 'root', traverse through each quad 
   * and draw an outline for it. The outline consists of the outermost pixels 
   * of the Quad (ie, the top and bottom rows, and the leftmost and rightmost 
   * columns).
   * 
   * Make sure that these outlines are of the input colour 'col' that is passed 
   * in. The colour of the remaining pixels should not be changed. 
   *
   * TODO: Implement this function
   */

  //Create variable that stores the index of a pixel
  int index = 0;

  //Utilize inorder traversal to set the colour for the pixels 
  if(root != NULL){
    drawOutline(im, root->left, col);
    for(int i = 0; i < root->w; i++){
      for(int j = 0; j < root->h; j++){
        //This conditional statement is to make sure the colours only change at the borders of the Quad
        if(i == 0 || i == root->w - 1 || j == 0 || j == root->h - 1){
          index = root->tx + i + ((root->ty + j) * im->sx);
          im->data[index] = col;
        }
      }
    }
    drawOutline(im, root->right, col); 
  }
  return;
}

///////////////////////////////////////////////////////////////////////////////

void save_Quad(Image *im, Quad *root) {
  /**
   * Given an image 'im' and a BST rooted at 'root', traverse through each 
   * quad, and set all the pixels in the corresponding area to the expected 
   * colour of the quad computed by your function get_colour().
   *
   * Make sure you index into the pixels array correctly and change the colour 
   * in the image itself.
   *
   * TODO: Implement this function
   */

  //Create a variable that will store the colour of the pixels for a Quad
  int colour = 0;
  //Create a variable that will store the index of a pixel
  int index = 0;

  //Utilize inorder traversal to set the colour for the pixels in a Quad
  if(root != NULL){
    save_Quad(im, root->left);     
    colour = get_colour(im, root);
    //Loop through the pixels in the Quad and set their im->data[index] to be the correct colour
    for(int i = 0; i < root->w; i++){
      for(int j = 0; j < root->h; j++){
        index = root->tx + i + ((root->ty + j) * im->sx); 
        im->data[index] = colour;
      }
    }            
    save_Quad(im, root->right);  
  }
  return;
}

///////////////////////////////////////////////////////////////////////////////
// That's it, you're done!
