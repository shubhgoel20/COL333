#pragma once

#include <vector>
#include <cstdlib>
#include <cassert>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>

//########################################### <Matrix Class> ############################# 
class Matrix{
    public:
        std::vector<std::vector<double>> mat;
        std::vector<int> shape;
        Matrix(){}
        Matrix(std::vector<std::vector<double>> mat);
        Matrix(int n,int m);
        Matrix(double f);
        Matrix(std::vector<double> v);
        Matrix(const Matrix &m);
    

        void print_mat(){
            for(int i = 0;i<shape[0];i++){
                for(int j = 0;j<shape[1];j++){
                    std::cout<<mat[i][j]<<" ";
                }
                std::cout<<std::endl;
            }
        }

        void print_mat(std::ofstream &out_file){
            for(int i = 0;i<shape[0];i++){
                for(int j = 0;j<shape[1];j++){
                    out_file<<mat[i][j];
                    if(j != shape[1] - 1) out_file<<" ";
                }
                out_file<<std::endl;
            }
        }

        double sum(){
            double ans = 0.0;
            for(int i = 0;i<shape[0];i++){
                for(int j = 0;j<shape[1];j++){
                    ans+=mat[i][j];
                }
            }
            return ans;
        }

        static Matrix matmul(Matrix &m1, Matrix &m2);
        static Matrix prod(Matrix &m1, Matrix &m2);
        static Matrix prod(double f, Matrix &m);
        static Matrix add(Matrix &m1, Matrix &m2);
        static Matrix add(double f, Matrix &m);
        static Matrix sub(Matrix &m1, Matrix &m2);
        static Matrix sub(double f, Matrix &m);
        static Matrix transpose(Matrix &m);
};

Matrix::Matrix(std::vector<double> v){
    this->shape.push_back(v.size());
    this->shape.push_back(1);
    // std::cout<<"here"<<"\n";
    std::vector<double> temp;
    for(auto x : v){
        // std::cout<<x<<"\n";
        temp.clear();
        temp.push_back(x);
        this->mat.push_back(temp);
    }
}

Matrix::Matrix(double f){
    this->shape.push_back(1);
    this->shape.push_back(1);
    std::vector<double> temp;
    temp.push_back(f);
    this->mat.push_back(temp);
}

Matrix::Matrix(const Matrix &m){
    this->shape = m.shape;
    std::vector<double> temp;
    for(int i = 0;i<m.shape[0];i++){
        temp.clear();
        for(int j = 0;j<m.shape[1];j++){
            temp.push_back(m.mat[i][j]);
        }
        this->mat.push_back(temp);
    }
}

Matrix::Matrix(std::vector<std::vector<double>> mat){
    this->mat = mat;
    shape.push_back(mat.size());
    shape.push_back(mat[0].size());
}

Matrix::Matrix(int n, int m){
    shape.push_back(n);
    shape.push_back(m);
    std::vector<double> temp;
    for(int i = 0;i<n;i++){
        temp.clear();
        for(int j = 0;j<m;j++){
            temp.push_back((std::rand()/((double)RAND_MAX)));
        }
        this->mat.push_back(temp);        
    }
}

Matrix Matrix::transpose(Matrix &m){
    Matrix res(m.shape[1],m.shape[0]);
    for(int i = 0;i<m.shape[0];i++){
        for(int j = 0;j<m.shape[1];j++){
            res.mat[j][i] = m.mat[i][j];
        }
    }
    return res;
}

Matrix Matrix::matmul(Matrix &m1, Matrix &m2){
    assert(m1.shape[1] == m2.shape[0]);
    Matrix res(m1.shape[0],m2.shape[1]);
    for(int i = 0;i<m1.shape[0];i++){
        for(int j = 0;j<m2.shape[1];j++){
            res.mat[i][j] = 0.0;
            for(int k = 0;k<m1.shape[1];k++){
                res.mat[i][j]+=m1.mat[i][k]*m2.mat[k][j];
            }
        }
    }
    return res;
}

Matrix Matrix::prod(Matrix &m1, Matrix &m2){
    assert(m1.shape == m2.shape);
    Matrix res(m1.shape[0],m1.shape[1]);
    for(int i = 0;i<m1.shape[0];i++){
        for(int j = 0;j<m1.shape[1];j++){
            res.mat[i][j] = m1.mat[i][j]*m2.mat[i][j];
        }
    }
    return res;
}

Matrix Matrix::prod(double f, Matrix &m){
    Matrix res(m.shape[0],m.shape[1]);
    for(int i = 0;i<m.shape[0];i++){
        for(int j = 0;j<m.shape[1];j++){
            res.mat[i][j] = m.mat[i][j]*f;
        }
    }
    return res;
}

Matrix Matrix::add(Matrix &m1, Matrix &m2){
    assert(m1.shape == m2.shape);
    Matrix res(m1.shape[0],m1.shape[1]);
    for(int i = 0;i<m1.shape[0];i++){
        for(int j = 0;j<m1.shape[1];j++){
            res.mat[i][j] = m1.mat[i][j]+m2.mat[i][j];
        }
    }
    return res;
}

Matrix Matrix::add(double f, Matrix &m){
    Matrix res(m.shape[0],m.shape[1]);
    for(int i = 0;i<m.shape[0];i++){
        for(int j = 0;j<m.shape[1];j++){
            res.mat[i][j] = m.mat[i][j]+f;
        }
    }
    return res;
}

Matrix Matrix::sub(Matrix &m1, Matrix &m2){
    assert(m1.shape == m2.shape);
    Matrix res(m1.shape[0],m1.shape[1]);
    for(int i = 0;i<m1.shape[0];i++){
        for(int j = 0;j<m1.shape[1];j++){
            res.mat[i][j] = m1.mat[i][j]-m2.mat[i][j];
        }
    }
    return res;
}

Matrix Matrix::sub(double f, Matrix &m){
    Matrix res(m.shape[0],m.shape[1]);
    for(int i = 0;i<m.shape[0];i++){
        for(int j = 0;j<m.shape[1];j++){
            res.mat[i][j] = f - m.mat[i][j];
        }
    }
    return res;
}

//########################################### </Matrix Class> #############################

//########################################### <Activations Class> #############################

class Activations{
    public:
        Activations(){}
        static Matrix tanh(Matrix &m);
        static Matrix tanh_grad(Matrix &m);
        static Matrix relu(Matrix &m);
        static Matrix relu_grad(Matrix &m);
};

Matrix Activations::tanh(Matrix &m){
    Matrix res(m);
    for(int i = 0;i<m.shape[0];i++){
        for(int j = 0;j<m.shape[1];j++){
            res.mat[i][j] = std::tanh(m.mat[i][j]);
        }
    }
    return res;
}

Matrix Activations::relu(Matrix &m){
    Matrix res(m);
    for(int i = 0;i<m.shape[0];i++){
        for(int j = 0;j<m.shape[1];j++){
            res.mat[i][j] = m.mat[i][j] > 0 ? m.mat[i][j]:0;
        }
    }
    return res;
}

Matrix Activations::tanh_grad(Matrix &m){
    Matrix res(m);
    for(int i = 0;i<m.shape[0];i++){
        for(int j = 0;j<m.shape[1];j++){
            res.mat[i][j] = 1 - (m.mat[i][j]*m.mat[i][j]);
        }
    }
    return res;
}

Matrix Activations::relu_grad(Matrix &m){
    Matrix res(m);
    for(int i = 0;i<m.shape[0];i++){
        for(int j = 0;j<m.shape[1];j++){
            res.mat[i][j] = m.mat[i][j] > 0 ? 1.0:0.0;
        }
    }
    return res;
}
//########################################### </Activations Class> #############################
class Layer{
    public:
        //grad assist variables;
        Matrix d,a,dd,da;
        double sum_a = 1.0;
        std::string type;
        std::string activation;
        int inp_neurons;
        int op_neurons;
        Matrix w;
        Matrix b;
        Matrix val;

        Matrix dw;
        Matrix db;
        Matrix dval;

        Layer(){}
        Layer(const Layer &l){
            inp_neurons = l.inp_neurons;
            op_neurons = l.op_neurons;
            type = l.type;
            activation = l.activation;
            w = l.w;
            b = l.b;
            val = l.val;

            dw = l.dw;
            db = l.db;
            dval = l.dval;

        }
        Layer(int inp_neurons, int op_neurons, std::string type = "hidden", std::string activation = "tanh"){
            this->inp_neurons = inp_neurons;
            this->op_neurons = op_neurons;
            this->type = type;
            this->activation = activation;
            w = Matrix(op_neurons,inp_neurons);
            b = Matrix(op_neurons,1);
        }

        void forward(Matrix &x);
        Matrix calc_grad(Matrix &x);

};
//########################################### <Optimizers Class> #############################

class Optimizers{
    public:
        double lr;
        std::string opt = "None";
        Optimizers(){};
        Optimizers(double lr = 0.1, std::string opt = "SGD"){this->lr = lr;this->opt = opt;}
        void SGD(Layer &l){
            Matrix temp1 = Matrix::prod(lr,l.dw);
            l.w = Matrix::add(l.w,temp1);
            Matrix temp2 = Matrix::prod(lr,l.db);
            l.b = Matrix::add(l.b,temp2);
        }
        void update(Layer &l){
            if(opt == "SGD"){
                SGD(l);
            }
            
        }
};

//########################################### </Optimizers Class> #############################

//########################################### <Loss Class> #############################
class Loss{
    public:
    static double MSE(Matrix &yhat, Matrix &y){
        assert(yhat.shape == y.shape);
        double loss = 0.0;
        double n = (double)y.shape[0];
        Matrix temp1 = Matrix::sub(y,yhat);
        Matrix temp2 = Matrix::prod(temp1,temp1);
        Matrix temp = Matrix::prod((1.0/n),temp2);
        for(int i = 0;i<temp.shape[0];i++){
            for(int j = 0;j<temp.shape[1];j++){
                loss+= temp.mat[i][j];
            }
        }
        return loss;
    }

    static Matrix MSE_grad(Matrix &yhat, Matrix &y){
        assert(yhat.shape == y.shape);
        double n = (double)y.shape[0];
        Matrix temp1 = Matrix::sub(y,yhat);
        Matrix grad = Matrix::prod((2.0/n),temp1);
        return grad;
    }

    static double get_loss(std::string lossfn, Matrix &yhat, Matrix &y){
        if(lossfn == "MSE"){
            return MSE(yhat,y);
        }
        else MSE(yhat,y);
        return 0.0;
    }

    static Matrix get_grad(std::string lossfn, Matrix &yhat, Matrix &y){
        Matrix grad;
        if(lossfn == "MSE"){
            grad = MSE_grad(yhat,y);
        }
        else grad = MSE_grad(yhat,y);
        return grad;
    }

};

//########################################### </Loss Class> #############################

//########################################### <Layer Class> #############################
void Layer::forward(Matrix &x){
    d = Matrix::matmul(w,x);
    a = Matrix::add(d,b);
    val = (type == "output") ? a: Activations::relu(a); //change this according to activation variable;
}

Matrix Layer::calc_grad(Matrix &x){
    auto temp = (type == "output") ? 1.0:Activations::relu_grad(val);
    da = Matrix::prod(temp,dval);
    db = Matrix(da);
    dd = Matrix(da);
    Matrix x_T = Matrix::transpose(x);
    dw = Matrix::matmul(dd,x_T);

    Matrix w_T = Matrix::transpose(w);
    Matrix dx = Matrix::matmul(w_T,dd);
    return dx;
}
//########################################### </Layer Class> #############################


//########################################### <NN Class> #############################

class NN{
    public:
        std::vector<Layer> layers;
        std::vector<int> architecture;
        int inp_size;
        int op_size;
        NN(){};
        NN(std::vector<int> &architecture, int inp_size, int op_size){
            assert(architecture.size() >= 1);
            this->architecture = std::vector<int>(architecture.begin(),architecture.end());
            this->inp_size = inp_size;
            this->op_size = op_size;
            Layer input_layer(inp_size,architecture[0],"input");
            layers.push_back(input_layer);
            for(int i = 0;i<architecture.size() - 1;i++){
                Layer layer(architecture[i],architecture[i+1]);
                layers.push_back(layer);
            }
            Layer output_layer(architecture[architecture.size() - 1],op_size,"output"); 
            layers.push_back(output_layer);
        }

        void save_model(std::string model_path);
        void load_model(std::string model_path);
        Matrix forward_prop(Matrix &input);
        void back_prop(Matrix &input, Matrix &dy, Optimizers &opt);
        void fit(std::vector<Matrix> &X_train, std::vector<Matrix> &y_train, int epochs, Optimizers &opt, std::string lossfn, bool logs = true);
};

void NN::load_model(std::string model_path){
    std::ifstream file(model_path);
    if(file.is_open()){
        std::string line;
        getline(file,line);
        std::stringstream ss;
        ss.str(line);
        ss>>inp_size>>op_size;
        int num_layers;
        ss>>num_layers;
        getline(file,line);
        std::stringstream ss1;
        ss1.str(line);
        architecture.clear();
        for(int i = 0;i<num_layers;i++){
            int temp;
            ss1>>temp;
            architecture.push_back(temp);
        }
        layers.clear();
        Layer input_layer(inp_size,architecture[0],"input");
        layers.push_back(input_layer);
        for(int i = 0;i<architecture.size() - 1;i++){
            Layer layer(architecture[i],architecture[i+1]);
            layers.push_back(layer);
        }
        Layer output_layer(architecture[architecture.size() - 1],op_size,"output"); 
        layers.push_back(output_layer);
        for(auto &layer: layers){
            for(int row = 0;row<layer.op_neurons;row++){
                getline(file,line);
                std::stringstream ss2;
                ss2.str(line);
                for(int col = 0;col<layer.inp_neurons;col++){
                    ss2>>layer.w.mat[row][col];
                }
            }
            for(int row = 0;row<layer.op_neurons;row++){
                getline(file,line);
                std::stringstream ss3;
                ss3.str(line);
                for(int col = 0;col<1;col++){
                    ss3>>layer.b.mat[row][col];
                }
            }            
        }
    }
    else{
        std::cout<<"Load Model failed"<<std::endl;
    }
}

void NN::save_model(std::string model_path){
    std::ofstream out_file;
    out_file.open(model_path, std::ios::out | std::ios::trunc);
    out_file<<inp_size<<" "<<op_size<<" "<<architecture.size()<<"\n";
    for(int i = 0;i<architecture.size();i++){
        out_file<<architecture[i];
        if(i != architecture.size() - 1) out_file<<" ";
    }
    out_file<<"\n";
    for(auto &layer: layers){
        layer.w.print_mat(out_file);
        layer.b.print_mat(out_file);
    }
    out_file.close();
}

Matrix NN::forward_prop(Matrix &input){
    assert(input.shape[0] == inp_size);
    layers[0].forward(input);
    for(int i = 1;i<layers.size();i++){
        layers[i].forward(layers[i-1].val);
    }
    return layers[layers.size() - 1].val;
}

void NN::back_prop(Matrix &input, Matrix &dy, Optimizers &opt){
    assert(input.shape[0] == inp_size);
    assert(dy.shape[0] == op_size);
    layers[layers.size() - 1].dval = dy;
    for(int i = layers.size() - 1;i>0;i--){
        layers[i-1].dval = layers[i].calc_grad(layers[i-1].val);
        opt.update(layers[i]);
    }
    layers[0].calc_grad(input);
    opt.update(layers[0]);
}

void NN::fit(std::vector<Matrix> &X_train, std::vector<Matrix> &y_train, int epochs, Optimizers &opt, std::string lossfn, bool logs){
    assert(X_train.size() == y_train.size());
    for(int epoch = 1;epoch <=epochs;epoch++){
        double epoch_loss = 0.0;
        for(int i = 0;i<X_train.size();i++){
            auto yhat = forward_prop(X_train[i]);
            auto dy = Loss::get_grad(lossfn,yhat,y_train[i]);
            epoch_loss+=Loss::get_loss(lossfn,yhat,y_train[i]);
            back_prop(X_train[i], dy, opt);
        }
        if(logs){
            std::cout<<"Epoch: "<<epoch<<" , Loss: "<<(epoch_loss/X_train.size())<<std::endl;
        }
    }
}

//########################################### </NN Class> #############################
