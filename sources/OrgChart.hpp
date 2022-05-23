#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace ariel{
    class OrgChart
    {
        private:
            //node class that save the date and pointer to other nodes
            struct ONode
            {
                std::string data;
                ONode* father;
                ONode* child;
                ONode* brother;
                ONode(std::string& data)
                {
                    this->data = data;
                    this->father = nullptr;
                    this->child = nullptr;
                    this->brother = nullptr;
                };
            };
            
        ONode* root;

        public:
        //constarctor
        OrgChart();

        //copy constractor
        OrgChart(OrgChart& temp_tree);

        ~OrgChart();

        //recursive function to remove all nodes
        void distractor_helper(ONode* curr);


        //dipp copy
        OrgChart& operator= (const OrgChart& temp_tree);
        
        //copy helper 
        void copy_helper (ONode* new_node, ONode* old_node);
        
        OrgChart& add_root(std::string data);

        //add children
        OrgChart& add_sub(const std::string& father, std::string son);

        //find node with the same data
        ONode* find(const std::string& data);
        //print
        friend std::ostream& operator<<(std::ostream& My_output,const OrgChart& tree);
    ////////////////// level_order ////////////////////
    class Iterator_level_order
        {
            ONode * ptr;
            std::vector<ONode*> ptr_vec;
        
            public:
                Iterator_level_order(ONode* new_ptr)
                {
                    this->ptr = new_ptr;
                }
                std::string& operator*();

                Iterator_level_order& operator++();
                
                Iterator_level_order operator++(int);
                
                bool operator!=(const Iterator_level_order &other) const ;

                std::string* operator->();

                ONode* get_ptr();

    };
    
    //////////////////reverse_order////////////////////
    class Iterator_reverse_order
        {
            ONode * ptr;
            std::vector<ONode*> ptr_vec;
            ulong place;
        
            public:
                Iterator_reverse_order(ONode* new_ptr)
                {
                    this->ptr = new_ptr;
                    this->place = 0;
                }
                std::string& operator*();

                Iterator_reverse_order& operator++();
                
                Iterator_reverse_order operator++(int);
                
                bool operator!=(const Iterator_reverse_order &other) const ;
                
                Iterator_reverse_order& full_stack();

                std::string* operator->();

                ONode* get_ptr();

    };
    
    /////////////////preorder//////////////////////////
    class Iterator_preorder
        {
            ONode * ptr;
            public:
                Iterator_preorder(ONode* new_ptr)
                {
                    this->ptr = new_ptr;
                }
                std::string& operator*();

                Iterator_preorder& operator++();
                
                Iterator_preorder operator++(int);
                
                bool operator!=(const Iterator_preorder &other) const ;
                //function that help to full the vector

                std::string* operator->();

                ONode* get_ptr();

    };
    
    Iterator_level_order begin();

    Iterator_level_order end();
    // level order
    Iterator_level_order begin_level_order();

    Iterator_level_order end_level_order();

    //revers order
    Iterator_reverse_order begin_reverse_order();

    Iterator_reverse_order reverse_order();
    
    //preorder order
    Iterator_preorder begin_preorder();

    Iterator_preorder end_preorder();

    };
}

