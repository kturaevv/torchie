#include <algorithm>
#include <cctype>
#include <ranges>
#include <string>

#include "tensor.hpp"
#include "tensor_autodiff.hpp"
#include "tensor_data.hpp"
#include "tensor_functions.hpp"
#include "utils.hpp"

namespace tensor {
    using namespace tensor_autodiff;

    Shape Tensor::shape() const {
        return this->data.shape;
    }

    Tensor Tensor::zeros(Shape shape) {
        return Tensor(TensorData(utils::zeros(shape), shape));
    }

    Tensor Tensor::zeros() const {
        return Tensor::zeros(this->shape());
    }

    TensorDataInfo Tensor::info() const {
        return this->data.info();
    }

    std::vector<Tensor*> Tensor::parents() const {
        return this->history.inputs;
    }

    bool Tensor::is_leaf() {
        return parents().empty();
    }

    void Tensor::accumulate_grad(Tensor& deriv) {
        (*this->grad) += deriv;
        return;
    }

    // std::vector<std::tuple<Tensor*, Tensor>> Tensor::chain_rule(Tensor*
    // deriv) {
    //     History history             = this->history;
    //     std::array<Tensor, 2> grads = history.backward(history.ctx, deriv);

    //     std::vector<std::tuple<Tensor*, Tensor>> vals;
    //     // for (size_t i = 0; i < history.inputs.size() && i < 2; i++)
    //     //     vals.emplace_back(history.inputs[i], std::move(grads[i]));

    //     return vals;
    // }

    void Tensor::backward() {
        auto deriv = new Tensor({ 1.0 });
        tensor_autodiff::backpropagate(this, deriv);
        return;
    }

}  // namespace tensor