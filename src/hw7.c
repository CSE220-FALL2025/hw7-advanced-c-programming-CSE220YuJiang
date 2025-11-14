#include "hw7.h"

/*
 * Insert one matrix into BST.
 * If root is NULL, create a new node.
 * Use mat->name (a char) as the key to go left / right.
 */
bst_sf *insert_bst_sf(matrix_sf *mat, bst_sf *root)
{
    if (root == NULL)
    {
        bst_sf *node = malloc(sizeof(bst_sf));
        if (node == NULL)
        {

            return NULL;
        }

        node->mat = mat;
        node->left_child = NULL;
        node->right_child = NULL;

        return node;
    }

    if (mat->name < root->mat->name)
    {
        root->left_child = insert_bst_sf(mat, root->left_child);
    }
    else if (mat->name > root->mat->name)
    {
        root->right_child = insert_bst_sf(mat, root->right_child);
    }

    return root;
}

/*
 * Search a BST for a matrix with the given name.
 * Return pointer to that matrix, or NULL if not found.
 */
matrix_sf *find_bst_sf(char name, bst_sf *root)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (name == root->mat->name)
    {
        return root->mat;
    }
    else if (name < root->mat->name)
    {

        return find_bst_sf(name, root->left_child);
    }
    else
    {
        return find_bst_sf(name, root->right_child);
    }
}

/*
 * Free the whole BST and all matrices stored inside it.
 * Safe to call with root == NULL.
 */
void free_bst_sf(bst_sf *root)
{
    if (root == NULL)
    {
        return;
    }
    free_bst_sf(root->left_child);
    free_bst_sf(root->right_child);
    free(root->mat);
    free(root);
}

/*
 * Add two matrices mat1 + mat2, element by element.
 * Return a new heap-allocated matrix with the sum.
 */
matrix_sf *add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2)
{
    unsigned int rows = mat1->num_rows;
    unsigned int cols = mat1->num_cols;

    matrix_sf *m;

    unsigned int cellCount = rows * cols;
    unsigned int bytesForValues = cellCount * sizeof(int);
    unsigned int totalBytes = sizeof(matrix_sf) + bytesForValues;

    m = (matrix_sf *)malloc(totalBytes);

    if (m == NULL)
    {
        return NULL;
    }

    m->name = 'p';
    m->num_rows = rows;
    m->num_cols = cols;

    unsigned int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            unsigned int k = i * cols + j;
            m->values[k] = mat1->values[k] + mat2->values[k];
        }
    }

    return m;
}

/*
 * Multiply two matrices mat1 * mat2 using triple nested loops.
 * Return a new heap-allocated matrix with the product.
 */
matrix_sf *mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2)
{
    unsigned int r1 = mat1->num_rows;
    unsigned int c1 = mat1->num_cols;
    unsigned int r2 = mat2->num_rows;
    unsigned int c2 = mat2->num_cols;

    matrix_sf *m;
    unsigned int cellCount = r1 * c2;
    unsigned int bytesForValues = cellCount * sizeof(int);
    unsigned int totalBytes = sizeof(matrix_sf) + bytesForValues;

    m = (matrix_sf *)malloc(totalBytes);
    if (m == NULL)
    {
        return NULL;
    }

    m->name = 'a';
    m->num_rows = r1;
    m->num_cols = c2;
    matrix_sf *m = NULL;
    unsigned int i, j, k;

    for (i = 0; i < r1; i++)
    {
        for (j = 0; j < c2; j++)
        {
            int sum = 0;

            for (k = 0; k < c1; k++)
            {
                unsigned int idx1 = i * c1 + k;
                unsigned int idx2 = k * c2 + j;
                sum += mat1->values[idx1] * mat2->values[idx2];
            }

            unsigned int idxRes = i * c2 + j;
            m->values[idxRes] = sum;
        }
    }

    return m;
}

/*
 * Return the transpose of a matrix.
 * Create a new matrix and swap rows/cols positions in the flat array.
 */
matrix_sf *transpose_mat_sf(const matrix_sf *mat)
{
    unsigned int rows = mat->num_rows;
    unsigned int cols = mat->num_cols;

    matrix_sf *m;

    unsigned int cellCount = rows * cols;
    unsigned int bytesForValues = cellCount * sizeof(int);
    unsigned int totalBytes = sizeof(matrix_sf) + bytesForValues;

    m = (matrix_sf *)malloc(totalBytes);

    if (m == NULL)
    {
        return NULL;
    }

    m->name = 'p';
    m->num_rows = rows;
    m->num_cols = cols;

    unsigned int i, j;

    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            unsigned int src = i * cols + j;
            unsigned int dst = j * rows + i;
            m->values[dst] = mat->values[src];
        }
    }

    return m;
}


/*
 * Parse a string like "3 4 [ 1 2 ; 3 4 ; ... ]"
 * First read num_rows and num_cols, then read all integer values
 * into the matrix in row-major order.
 */
matrix_sf *create_matrix_sf(char name, const char *expr)
{
    const char *p = expr;
    unsigned int rows = 0;
    unsigned int cols = 0;
    int stage = 0;

    while (*p != '\0' && *p != '[')
    {
        char ch = *p;

        if (ch >= '0' && ch <= '9')
        {
            int digit = ch - '0';
            if (stage == 0)
            {
                rows = rows * 10 + digit;
            }
            else if (stage == 1)
            {
                cols = cols * 10 + digit;
            }
        }
        else if ((ch == ' ' || ch == '\t') && stage == 0 && rows > 0)
        {
            stage = 1;
        }

        p++;
    }

    if (*p != '[')
    {
        return NULL;
    }
    p++;

    unsigned int cellCount = rows * cols;
    unsigned int bytesForValues = cellCount * sizeof(int);
    unsigned int totalBytes = sizeof(matrix_sf) + bytesForValues;

    matrix_sf *m = (matrix_sf *)malloc(totalBytes);
    if (m == NULL)
    {
        return NULL;
    }

    m->name = name;
    m->num_rows = rows;
    m->num_cols = cols;

    unsigned int idx = 0;
    int currentValue = 0;
    int inNumber = 0;
    int isNegative = 0;

    while (*p != '\0' && *p != ']')
    {
        char ch = *p;

        if (ch == '-')
        {
            isNegative = 1;
            currentValue = 0;
            inNumber = 1;
        }
        else if (ch >= '0' && ch <= '9')
        {
            int digit = ch - '0';
            currentValue = currentValue * 10 + digit;
            inNumber = 1;
        }
        else
        {
            if (inNumber)
            {
                if (isNegative)
                {
                    currentValue = -currentValue;
                }

                if (idx < rows * cols)
                {
                    m->values[idx] = currentValue;
                    idx++;
                }

                currentValue = 0;
                inNumber = 0;
                isNegative = 0;
            }

            if (ch == ';')
            {
            }
        }

        p++;
    }

    if (inNumber && idx < rows * cols)
    {
        if (isNegative)
        {
            currentValue = -currentValue;
        }
        m->values[idx] = currentValue;
        idx++;
    }

    return m;
}

/*
 * Convert an infix matrix expression (A+B*C, etc.)
 * into a postfix string using a simple operator stack.
 * Returns a heap-allocated C-string.
 */
char *infix2postfix_sf(char *infix)
{
    int n = strlen(infix);
    char *postfix = malloc(n + 1);
    int out = 0;

    char op_stack[256];
    int top = -1;

    for (int i = 0; infix[i] != '\0'; i++)
    {
        char ch = infix[i];

        if (ch >= 'A' && ch <= 'Z')
        {
            postfix[out] = ch;
            out++;
        }
        else if (ch == '\'')
        {
            postfix[out] = ch;
            out++;
        }
        else if (ch == '(')
        {
            top++;
            op_stack[top] = ch;
        }
        else if (ch == ')')
        {
            while (top >= 0 && op_stack[top] != '(')
            {
                postfix[out] = op_stack[top];
                out++;
                top--;
            }
            if (top >= 0 && op_stack[top] == '(')
            {
                top--;
            }
        }
        else if (ch == '+' || ch == '*')
        {
            while (top >= 0 && op_stack[top] != '(')
            {
                char t = op_stack[top];
                int top_prec = 0;
                if (t == '+')
                {
                    top_prec = 1;
                }
                else if (t == '*')
                {
                    top_prec = 2;
                }

                int ch_prec = 0;
                if (ch == '+')
                {
                    ch_prec = 1;
                }
                else if (ch == '*')
                {
                    ch_prec = 2;
                }

                if (top_prec >= ch_prec)
                {
                    postfix[out] = op_stack[top];
                    out++;
                    top--;
                }
                else
                {
                    break;
                }
            }

            top++;
            op_stack[top] = ch;
        }
        else
        {
        }
    }

    while (top >= 0)
    {
        if (op_stack[top] != '(')
        {
            postfix[out] = op_stack[top];
            out++;
        }
        top--;
    }

    postfix[out] = '\0';
    return postfix;
}

matrix_sf *evaluate_expr_sf(char name, char *expr, bst_sf *root)
{
    return NULL;
}

matrix_sf *execute_script_sf(char *filename)
{
    return NULL;
}

// This is a utility function used during testing. Feel free to adapt the code to implement some of
// the assignment. Feel equally free to ignore it.
matrix_sf *copy_matrix(unsigned int num_rows, unsigned int num_cols, int values[])
{
    matrix_sf *m = malloc(sizeof(matrix_sf) + num_rows * num_cols * sizeof(int));
    m->name = '?';
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    memcpy(m->values, values, num_rows * num_cols * sizeof(int));
    return m;
}

// Don't touch this function. It's used by the testing framework.
// It's been left here in case it helps you debug and test your code.
void print_matrix_sf(matrix_sf *mat)
{
    assert(mat != NULL);
    assert(mat->num_rows <= 1000);
    assert(mat->num_cols <= 1000);
    printf("%d %d ", mat->num_rows, mat->num_cols);
    for (unsigned int i = 0; i < mat->num_rows * mat->num_cols; i++)
    {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows * mat->num_cols - 1)
            printf(" ");
    }
    printf("\n");
}
