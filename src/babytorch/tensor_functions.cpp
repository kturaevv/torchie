#include "operators.hpp"
#include "tensor.hpp"
#include "tensor_functions.hpp"

namespace tensor_functions {

    using tensor::Tensor;
    using tensor_autodiff::Context;

    Tensor Add::forward(Context&, const Tensor& self, const Tensor& other) {
        return self.backend.add_zip(self, other);
    }

    std::array<Tensor, 2> Add::backward(Context&, const Tensor& d_out) {
        return { d_out, d_out };
    }

    Tensor Neg::forward(Context& ctx, const Tensor& self) {
        ctx.save_for_backwards(self);
        return self.backend.neg_map(self);
    }

    std::array<Tensor, 2> Neg::backward(Context&, const Tensor& d_out) {
        return { d_out.backend.neg_map(d_out) };
    }

    Tensor Inv::forward(Context& ctx, const Tensor& self) {
        ctx.save_for_backwards(self);
        return self.backend.inv_map(self);
    }

    std::array<Tensor, 2> Inv::backward(Context& ctx, const Tensor& d_out) {
        auto self = ctx.saved_values[0];
        return { self.backend.inv_back_zip(self, d_out) };
    }

    Tensor Relu::forward(Context& ctx, const Tensor& self) {
        ctx.save_for_backwards(self);
        return self.backend.relu_map(self);
    }

    std::array<Tensor, 2> Relu::backward(Context& ctx, const Tensor& d_out) {
        auto self = ctx.saved_values[0];
        return { d_out.backend.relu_back_zip(self, d_out) };
    }

    Tensor Sigmoid::forward(Context& ctx, const Tensor& self) {
        ctx.save_for_backwards(self);
        return self.backend.sigmoid_map(self);
    }

    std::array<Tensor, 2> Sigmoid::backward(Context& ctx, const Tensor& d_out) {
        auto self            = ctx.saved_values[0];
        auto sigmoid_self    = self.backend.sigmoid_map(self);
        auto sigmoid_self_sq = self.backend.mul_zip(sigmoid_self, sigmoid_self);
        auto exp_minus_self  = self.backend.exp_map(self.backend.neg_map(self));
        auto mul_zip         = self.backend.mul_zip;
        auto out = mul_zip(d_out, mul_zip(exp_minus_self, sigmoid_self_sq));
        return { out };
    }

    Tensor Log::forward(Context& ctx, const Tensor& self) {
        ctx.save_for_backwards(self);
        return self.backend.log_map(self);
    }

    std::array<Tensor, 2> Log::backward(Context& ctx, const Tensor& d_out) {
        auto self = ctx.saved_values[0];
        return { self.backend.log_back_zip(self, d_out) };
    }

    Tensor Exp::forward(Context& ctx, const Tensor& self) {
        ctx.save_for_backwards(self);
        return self.backend.exp_map(self);
    }

    std::array<Tensor, 2> Exp::backward(Context& ctx, const Tensor& d_out) {
        auto self = ctx.saved_values[0];
        return { self.backend.mul_zip(d_out, self.backend.exp_map(self)) };
    }

    Tensor Mul::forward(Context& ctx, const Tensor& self, const Tensor& other) {
        ctx.save_for_backwards(self, other);
        return self.backend.mul_zip(self, other);
    }

    std::array<Tensor, 2> Mul::backward(Context& ctx, const Tensor& d_out) {
        auto self  = ctx.saved_values[0];
        auto other = ctx.saved_values[1];
        return { self.backend.mul_zip(other, d_out),
                 self.backend.mul_zip(self, d_out) };
    }

    Tensor Lt::forward(Context&, const Tensor& self, const Tensor& other) {
        return self.backend.lt_zip(self, other);
    }

    std::array<Tensor, 2> Lt::backward(Context&, const Tensor& d_out) {
        Tensor a = d_out.zeros();
        Tensor b = d_out.zeros();
        return { a, b };
    }

    Tensor Eq::forward(Context&, const Tensor& self, const Tensor& other) {
        return self.backend.eq_zip(self, other);
    }

    std::array<Tensor, 2> Eq::backward(Context&, const Tensor& d_out) {
        Tensor a = d_out.zeros();
        Tensor b = d_out.zeros();
        return { a, b };
    }

    Tensor Is_close::forward(Context& ctx,
                             const Tensor& self,
                             const Tensor& other) {
        ctx.save_for_backwards(self, other);
        return self.backend.is_close_zip(self, other);
    }

    Tensor Copy::forward(Context&, const Tensor& self) {
        return self.backend.id_map(self);
    }

    std::array<Tensor, 2> Copy::backward(Context&, const Tensor& d_out) {
        return { d_out };
    }

}  // namespace tensor_functions