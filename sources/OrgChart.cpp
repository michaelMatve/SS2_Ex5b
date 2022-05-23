#include "OrgChart.hpp"

namespace ariel{

//////////////////////////////////////////////////////////////////
///////////////OrgChart class ////////////////////////////////////
//////////////////////////////////////////////////////////////////
    //constractors

    OrgChart::OrgChart()
    {
        this->root = nullptr;//start root to null ptr
    }

    OrgChart::OrgChart(OrgChart& temp_tree)
    {
        if(temp_tree.root == nullptr)
        {
            this->root = nullptr;
        }
        else
        {
            this->root = new ONode(temp_tree.root->data);
            *this = temp_tree;
        }
    }
   
    //distractor
    OrgChart::~OrgChart()
    {
        distractor_helper(this->root);
    }

    void OrgChart::distractor_helper(ONode* curr)
    {
        if(curr != nullptr)
        {
            if(curr->brother != nullptr)
            {
                distractor_helper(curr->brother);
            }
            if(curr->child != nullptr)
            {
                distractor_helper(curr->child);
            }
            delete(curr);
        }
    }
    
    //operators for dip copy
    OrgChart& OrgChart::operator= (const OrgChart& temp_tree)
    {
        //check that they are not the same 
        if(this == &temp_tree)
        {
            return *this;
        }
        //check the root is not null
        if(temp_tree.root == nullptr)
        {
            this->root = nullptr;
        }
        else
        {
        this->root = new ONode(temp_tree.root->data);
        this->copy_helper(this->root,temp_tree.root);
        }
        return *this;
    }
    
    void OrgChart::copy_helper (ONode* new_node, ONode* old_node)
    {
        if(old_node == nullptr)
        {
            return;
        }
        //run the function on all the node children firest on his child then on the brothers
        if(old_node->child != nullptr)
        {
            new_node->child = new ONode(old_node->data);
            ONode* new_children = new_node->child;
            ONode* old_children = old_node->child;
            while (old_children!=nullptr)
            {
                new_children->father = old_node;
                copy_helper(new_children,old_children);
                 if(old_children->brother != nullptr)
                 {
                    new_children->brother =  new ONode(old_children->brother->data);
                 }
                new_children = new_children->brother;
                old_children = old_children->brother;
            }
        } 
    }
    
    //change root
    OrgChart& OrgChart::add_root(std::string data)
    {
        if(data.empty() || data == "\n"|| data == "\t")
        {
            throw std::invalid_argument("cant get that string");
        }
        //if the root is null make new node to the root
        if(this->root == nullptr)
        {
            this->root = new ONode{data};
        }
        //change the data
        this->root->data = data;
        return *this;
    }

    //add children
    OrgChart& OrgChart::add_sub(const std::string& father, std::string son)
    {
        if(son.empty() || son == "\n"|| son == "\t")
        {
            throw std::invalid_argument("cant get that string");
        }
        //find the father
        ONode* pointer_father = this->find(father);
        //if the pather not found throw exp
        if(pointer_father == nullptr)
        {
            throw std::invalid_argument("not find");
        }
        // make new node to the son
        ONode* pointer_son = new ONode(son);
        //set the son father
        pointer_son->father = pointer_father;
        //set the new son node as a fathers son as last shild
        if(pointer_father->child == nullptr)
        {
            //if the father as no child put this node as child
            pointer_father->child = pointer_son ;
        }
        else
        {
            //run on all the child antill the end and put it in the end
            pointer_father = pointer_father->child;
            while(pointer_father->brother != nullptr)
            {
                pointer_father = pointer_father->brother;
            }
            pointer_father->brother = pointer_son;
        }
        return *this;
    }

    OrgChart::ONode* OrgChart::find(const std::string& data)
    {
        //run on the level order operator and check if the data is the same 
        for (auto it = this->begin_level_order(); it != this->end_level_order(); ++it)
        {
            if(*it == data)
            {
                //return the node with the same data
                return it.get_ptr();
            }
        }
        return nullptr;
    }

    std::ostream& operator<<(std::ostream& My_output,const OrgChart& tree)
    {
        //start
        My_output<<"-----------------start----------"<<"\n";
        //counter to remember in what level we are
        int colums =0;

        OrgChart::ONode* curr_node = tree.root;
        //add the root to the my_output
        My_output<<" ("<<colums<< ") "<< curr_node->data;
        //run on all the nodes in predetor order and print them in to my output
       while(curr_node!= nullptr)
        {
            //run to the last level
            while(curr_node->child != nullptr)
            {
                curr_node= curr_node->child;
                ++colums;
                My_output<< " -> " <<" ("<<colums<< ") "<<curr_node->data;
            }
            //after finish go up to the prvius level and check if there is brother
            while(curr_node->brother == nullptr)
            {
                curr_node = curr_node->father;
                //if we get to null stop
                if(curr_node == nullptr)
                {
                    break;
                }
                --colums;
            }
            if(curr_node != nullptr)
            {
                curr_node = curr_node->brother;
                My_output<< "\n";
                for(ulong i= 0 ; i<colums ; i++)
                {
                   My_output<<" \t  ";  
                }
                My_output<< " -> " <<" ("<<colums<< ") "<<curr_node->data;
            }
        }

        My_output<<"\n"<<"-----------------end----------"<<std::endl;
        return My_output;
    }
    //iterators start and end

    OrgChart::Iterator_level_order OrgChart::begin_level_order()
    {
        //return the start of the itrshion
        if(this->root == nullptr)
        {
            throw std::invalid_argument("null ptr");
        }
        return Iterator_level_order(this->root);
    }

    OrgChart::Iterator_level_order OrgChart::end_level_order()
    {
        if(this->root == nullptr)
        {
            throw std::invalid_argument("null ptr");
        }
        return nullptr;
    }
    
    OrgChart::Iterator_reverse_order OrgChart::begin_reverse_order()
    {
        if(this->root == nullptr)
        {
            throw std::invalid_argument("null ptr");
        }
        //return the start of the itrshion
        return Iterator_reverse_order(this->root).full_stack();
    }

    OrgChart::Iterator_reverse_order OrgChart::reverse_order()
    {
        if(this->root == nullptr)
        {
            throw std::invalid_argument("null ptr");
        }
        return nullptr;
    }

    OrgChart::Iterator_preorder OrgChart::begin_preorder()
    {
        if(this->root == nullptr)
        {
            throw std::invalid_argument("null ptr");
        }
        //return the start of the itrshion
        return Iterator_preorder(this->root);
    }

    OrgChart::Iterator_preorder OrgChart::end_preorder()
    {
        if(this->root == nullptr)
        {
            throw std::invalid_argument("null ptr");
        }
        return nullptr;
    }
 
    OrgChart::Iterator_level_order OrgChart::begin()
    {
        return this->begin_level_order();
    }

    OrgChart::Iterator_level_order OrgChart::end()
    {
        return this->end_level_order();
    }
 
 ////////////////level_order/////////////////////////////////
    std::string& OrgChart::Iterator_level_order::operator*()
    {
        return this->ptr->data;
    }

    std::string* OrgChart::Iterator_level_order::operator->()
    {
        return &(this->ptr->data);
    }
    
    OrgChart::Iterator_level_order& OrgChart::Iterator_level_order::operator++()
    {
            ONode* child = this->ptr->child;
            while (child!=nullptr)
            {
                this->ptr_vec.push_back(child);
                child = child->brother;
            }
        //if the vector is empty return nullptr get to end
        if(this->ptr_vec.empty())
        {
            this->ptr = nullptr;
        }
        //else set this->ptr to the ptr_vec in the place
        else
        {
            //change the pointer to the firest in the vector and remove it from the vector
            this->ptr = this->ptr_vec[0];
            this->ptr_vec.erase(this->ptr_vec.begin());
            //add all the node child pointers to the vector
        }
        return *this;
    }
    
    OrgChart::Iterator_level_order OrgChart::Iterator_level_order::operator++(int)
    {
        Iterator_level_order temp = *this;
        ++(*this);
        return temp;
    }
    
    bool OrgChart::Iterator_level_order::operator!=(const Iterator_level_order &other) const
    {
        //check the pointers are not the same
        return this->ptr!=other.ptr;
    }
     //return the iterators pointer to the node
    OrgChart::ONode* OrgChart::Iterator_level_order::get_ptr()
    {
        return this->ptr;
    }


    ////////////////Iterator_reverse_order/////////////////////////////////
    std::string& OrgChart::Iterator_reverse_order::operator*()
    {
        return this->ptr->data;
    }

    std::string* OrgChart::Iterator_reverse_order::operator->()
    {
        return &(this->ptr->data);
    }
    
    OrgChart::Iterator_reverse_order& OrgChart::Iterator_reverse_order::operator++()
    {
        //change the curr place
        ++this->place;
        //if the place is out of the vector range we finish end return null
        if(this->place == this->ptr_vec.size())
        {
            this->ptr = nullptr;
        }
        //else set this->ptr to the ptr_vec in the place
        else
        {
            this->ptr = this->ptr_vec[this->place];
        }
        return *this;
    }
    
    OrgChart::Iterator_reverse_order OrgChart::Iterator_reverse_order::operator++(int)
    {
        Iterator_reverse_order temp = *this;
        ++(*this);
        return temp;
    }
    
    bool OrgChart::Iterator_reverse_order::operator!=(const Iterator_reverse_order &other) const
    {
        //check the pointers are not the same
        return this->ptr!=other.ptr;
    }
    
    OrgChart::Iterator_reverse_order& OrgChart::Iterator_reverse_order::full_stack()
    {
        //check the tree is not null
        if(this->ptr == nullptr)
        {
            return *this;
        }
        //add the root at the begining
       this->ptr_vec.insert(this->ptr_vec.begin(),this->ptr);
       ulong tor_place = 0;
        //run on all the nodes in the vector from the end and add all the children of them in the begining
        while(tor_place < this->ptr_vec.size())
        {   
           ONode* curr_node = this->ptr_vec[this->ptr_vec.size()- tor_place -1]->child;
           //add the children in there true order
           int index =0;
           while(curr_node != nullptr)
           {
               this->ptr_vec.insert(this->ptr_vec.begin()+ index , curr_node);
               curr_node = curr_node->brother;
               ++index;
           }
           ++tor_place;
       }
       this->ptr = this->ptr_vec[0];
       return *this;
    }
     //return the iterators pointer to the node
    OrgChart::ONode* OrgChart::Iterator_reverse_order::get_ptr()
    {
        return this->ptr;
    }
 
 ////////////////Iterator_preorder/////////////////////////////////
    std::string& OrgChart::Iterator_preorder::operator*()
    {
        return this->ptr->data;
    }

    std::string* OrgChart::Iterator_preorder::operator->()
    {
        return &(this->ptr->data);
    }
    
    OrgChart::Iterator_preorder& OrgChart::Iterator_preorder::operator++()
    {
        //check if there is child
        if(this->ptr->child!= nullptr)
        {
           this->ptr = this->ptr->child;
           return *this;
        }
        //if there is brother return him else return null
        while(this->ptr->brother == nullptr)
        {
            //when no father return nullptr
            if(this->ptr->father == nullptr)
            {
                this->ptr = nullptr;
                return *this;
            }
            this->ptr = this->ptr->father;
        }
        this->ptr = this->ptr->brother;
        return *this;
    }
    
    OrgChart::Iterator_preorder OrgChart::Iterator_preorder::operator++(int)
    {
        Iterator_preorder temp = *this;
        ++(*this);
        return temp;
    }
    
    bool OrgChart::Iterator_preorder::operator!=(const Iterator_preorder &other) const
    {
        //check the pointers are not the same
        return this->ptr!=other.ptr;
    }
  
     //return the iterators pointer to the node
    OrgChart::ONode* OrgChart::Iterator_preorder::get_ptr()
    {
        return this->ptr;
    }

}