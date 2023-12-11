#include <vector>
#include <cstdlib>
#include <cassert>
#include <string>
#include <cmath>
#include <iostream>


//########################################### <Matrix Class> ############################# 
class Matrix{
    public:
        std::vector<std::vector<float>> mat;
        std::vector<int> shape;
        Matrix(){}
        Matrix(std::vector<std::vector<float>> mat);
        Matrix(int n,int m);
        Matrix(float f);
        Matrix(std::vector<float> v);
        Matrix(const Matrix &m);
    

        void print_mat(){
            for(int i = 0;i<shape[0];i++){
                for(int j = 0;j<shape[1];j++){
                    std::cout<<mat[i][j]<<" ";
                }
                std::cout<<std::endl;
            }
        }

        float sum(){
            float ans = 0.0;
            for(int i = 0;i<shape[0];i++){
                for(int j = 0;j<shape[1];j++){
                    ans+=mat[i][j];
                }
            }
            return ans;
        }

        static Matrix matmul(Matrix &m1, Matrix &m2);
        static Matrix prod(Matrix &m1, Matrix &m2);
        static Matrix prod(float f, Matrix &m);
        static Matrix add(Matrix &m1, Matrix &m2);
        static Matrix add(float f, Matrix &m);
        static Matrix sub(Matrix &m1, Matrix &m2);
        static Matrix sub(float f, Matrix &m);
        static Matrix transpose(Matrix &m);
};

Matrix::Matrix(std::vector<float> v){
    this->shape.push_back(v.size());
    this->shape.push_back(1);
    // std::cout<<"here"<<"\n";
    std::vector<float> temp;
    for(auto x : v){
        // std::cout<<x<<"\n";
        temp.clear();
        temp.push_back(x);
        this->mat.push_back(temp);
    }
}

Matrix::Matrix(float f){
    this->shape.push_back(1);
    this->shape.push_back(1);
    std::vector<float> temp;
    temp.push_back(f);
    this->mat.push_back(temp);
}

Matrix::Matrix(const Matrix &m){
    this->shape = m.shape;
    std::vector<float> temp;
    for(int i = 0;i<m.shape[0];i++){
        temp.clear();
        for(int j = 0;j<m.shape[1];j++){
            temp.push_back(m.mat[i][j]);
        }
        this->mat.push_back(temp);
    }
}

Matrix::Matrix(std::vector<std::vector<float>> mat){
    this->mat = mat;
    shape.push_back(mat.size());
    shape.push_back(mat[0].size());
}

Matrix::Matrix(int n, int m){
    shape.push_back(n);
    shape.push_back(m);
    std::vector<float> temp;
    for(int i = 0;i<n;i++){
        temp.clear();
        for(int j = 0;j<m;j++){
            temp.push_back((std::rand()/((float)RAND_MAX)));
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

Matrix Matrix::prod(float f, Matrix &m){
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

Matrix Matrix::add(float f, Matrix &m){
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

Matrix Matrix::sub(float f, Matrix &m){
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

//########################################### <Optimizers Class> #############################

class Optimizers{
    public:
        float lr;
        std::string opt = "None";
        Optimizers(){};
        Optimizers(float lr = 0.1, std::string opt = "SGD"){this->lr = lr;this->opt = opt;}
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
    static float MSE(Matrix &yhat, Matrix &y){
        assert(yhat.shape == y.shape);
        float loss = 0.0;
        float n = (float)y.shape[0];
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
        float n = (float)y.shape[0];
        Matrix temp1 = Matrix::sub(y,yhat);
        Matrix grad = Matrix::prod((2.0/n),temp1);
        return grad;
    }

    static float get_loss(std::string lossfn, Matrix &yhat, Matrix &y){
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
class Layer{
    public:
        //grad assist variables;
        Matrix d,a,dd,da;
        float sum_a = 1.0;
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
        int inp_size;
        int op_size;
        NN(){};
        NN(std::vector<int> &architecture, int inp_size, int op_size){
            assert(architecture.size() >= 1);
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

        Matrix forward_prop(Matrix &input);
        void back_prop(Matrix &input, Matrix &dy, Optimizers &opt);
        void fit(std::vector<Matrix> &X_train, std::vector<Matrix> &y_train, int epochs, Optimizers &opt, std::string lossfn, bool logs = true);
};

Matrix NN::forward_prop(Matrix &input){
    assert(input.shape[0] == inp_size);
    layers[0].forward(input);
    // std::cout<<"##############################################"<<"\n";
    // layers[0].val.print_mat();
    for(int i = 1;i<layers.size();i++){
        // std::cout<<"##############################################"<<"\n";
        layers[i].forward(layers[i-1].val);
        // layers[i].val.print_mat();
    }
    return layers[layers.size() - 1].val;
}

void NN::back_prop(Matrix &input, Matrix &dy, Optimizers &opt){
    // std::cout<<"###################### Back Prop ####################"<<"\n";
    assert(input.shape[0] == inp_size);
    assert(dy.shape[0] == op_size);
    layers[layers.size() - 1].dval = dy;
    // dy.print_mat();
    for(int i = layers.size() - 1;i>0;i--){
        // std::cout<<"##########################################"<<"\n";
        layers[i-1].dval = layers[i].calc_grad(layers[i-1].val);
        // layers[i].dw.print_mat();
        // std::cout<<"\n";
        // layers[i].db.print_mat();
        // std::cout<<"\n";
        opt.update(layers[i]);
    }
    //  std::cout<<"##########################################"<<"\n";
    layers[0].calc_grad(input);
    // layers[0].dw.print_mat();
    // std::cout<<"\n";
    // layers[0].db.print_mat();
    // std::cout<<"\n";
    opt.update(layers[0]);
}

void NN::fit(std::vector<Matrix> &X_train, std::vector<Matrix> &y_train, int epochs, Optimizers &opt, std::string lossfn, bool logs){
    assert(X_train.size() == y_train.size());
    for(int epoch = 1;epoch <=epochs;epoch++){
        float epoch_loss = 0.0;
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


int main(){
    // Matrix a(2,2);
    // Matrix b(2,2);

    // a.print_mat();
    // std::cout<<std::endl;
    // b.print_mat();

    // a = Matrix::sub(a,b);
    // std::cout<<std::endl;
    // a.print_mat();

    // Matrix c = Matrix::matmul(a,b);
    // a.print_mat();
    // std::cout<<std::endl;
    // b.print_mat();
    // std::cout<<std::endl;
    // c.print_mat();
    std::vector<Matrix> X_train;
    std::vector<Matrix> y_train;

    for(int i = 0;i<100;i++){
        int angle = std::rand()%10000;
        float val = std::sin(angle);
        // std::cout<<angle<<" "<<val<<"\n";
        std::vector<float> bits;
        for(int j = 15;j>=0;j--){
            float temp = 0.0;
            if(angle&(1<<j)) temp = 1.0;
            bits.push_back(temp);
        }
        Matrix x = Matrix(bits);
        Matrix y(val);
        // std::cout<<x.mat[1][0]<<"\n";
        X_train.push_back(x);
        y_train.push_back(y);
    }

    std::vector<int> arch({8,8,8});

    NN test(arch,16,1);
    // auto ytest = test.forward_prop(X_train[32]);
    
    // for(int i = 0;i<test.layers.size();i++){
    //     test.layers[i].val.print_mat();
    //     std::cout<<"----------------------------------------------------"<<std::endl;
    // }
    // for(int i = 0;i<test.layers.size();i++){
    //     auto layer = test.layers[i];
    //     layer.w.print_mat();
    //     std::cout<<std::endl;
    //     layer.b.print_mat();
    //     std::cout<<"----------------------------------------------------"<<std::endl;
    // }
    Optimizers opt(0.0001, "SGD");
    // // auto ytest = test.forward_prop(X_train[67]);
    // // std::cout<<ytest.mat[0][0];
    test.fit(X_train,y_train,2000,opt,"MSE");

    std::cout<<"#############################"<<"\n";

    std::vector<Matrix> X_test;
    std::vector<Matrix> y_test;


    for(int i = 0;i<100;i++){
        int angle = std::rand()%10000;
        float val = std::sin(angle);
        // std::cout<<angle<<" "<<val<<"\n";
        std::vector<float> bits;
        for(int j = 15;j>=0;j--){
            float temp = 0.0;
            if(angle&(1<<j)) temp = 1.0;
            bits.push_back(temp);
        }
        Matrix x = Matrix(bits);
        Matrix y(val);
        // std::cout<<x.mat[1][0]<<"\n";
        X_test.push_back(x);
        y_test.push_back(y);
    }

    
    for(int i = 0;i<100;i++){
        auto yhat = test.forward_prop(X_train[i]);
        std::cout<<"predicted: "<<yhat.mat[0][0]<<" , "<<"target: "<<y_train[i].mat[0][0]<<"\n";
    }

    return 0;
}