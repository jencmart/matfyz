import argparse
import datetime
import os
import re
import sys

import numpy as np
import tensorflow as tf


class DifferTest:

    def __init__(self):
        self._W1 = tf.Variable(tf.random.normal([3, 4], stddev=0.1, seed=17), trainable=True)
        self._b1 = tf.Variable(tf.ones([4]), trainable=True)

        self._W2 = tf.Variable(tf.random.normal([4, 2], stddev=0.1, seed=45), trainable=True)
        self._b2 = tf.Variable(tf.ones([2]), trainable=True)


    def get_Si(self, i, probabilities):
        return tf.gather_nd(probabilities, [0, i])

    def differentiate_softmax_wrt_logit(self, probabilities):

        # R^T -> R^T
        t = probabilities.shape[1]  # because it is row vector

        # output will be matrix (t,t)
        A = np.zeros((t, t))

        for i in range(t):
            for j in range(t):
                S_i = tf.gather_nd(probabilities, [0, i])
                S_j = tf.gather_nd(probabilities, [0, j])

                if i == j:
                    DjSi = S_i * (1.0 - S_j)
                else:
                    DjSi = -1.0 * S_j * S_i
                A[i, j] = DjSi  # podle i-teho outputu .. ano je to spravne

        return tf.constant(A)

    def differentiate_xW_by_W(self, x, W):
        DW = []

        # R^N -> R^T
        # first row -- whole W wrt first output

        # init empty array at first ...
        # output will be matrix (t,Nt)
        N = W.shape[0]
        t = W.shape[1]
        A = np.zeros((t, N*t))

        # set the correct values
        for i in range(t):
            for j in range(0,  N):
                A[i, i*N+j] = tf.gather_nd(x, [0, j])

        return tf.constant(A)

    def differentiate_xent_wrt_probabilites(self, onehot_y, probabilities):

        # vyrobime matici kde na radku vzdy jedno cislo == 1/-p_y
        vynulovane = tf.math.multiply(onehot_y, probabilities)
        delenec = tf.ones([vynulovane.shape[0], vynulovane.shape[1]], tf.float32)
        result = tf.math.scalar_mul(-1.0, tf.divide(delenec, vynulovane))
        result = tf.dtypes.cast(result, dtype=tf.float64)

        # remove nan or inf ...
        mask = tf.math.is_finite(result)
        result = tf.where(mask, result, tf.zeros_like(result))

        return result

    def differentiate_vect_tanh_wrt_vect(self, vector_before_tanh):
        #  tf.math.divide(tf.constant(1.0), tf.add(tf.constant(1.0), tf.exp(tf.negative(x))))
        # R^T -> R^T

        # output will be matrix (t,t)
        t = vector_before_tanh.shape[1]  # because it is row vector
        A = np.zeros((t, t))

        for i in range(t):
            cosh = np.cosh(tf.gather_nd(vector_before_tanh, [0, i]))  # todo  I think nonzero only on diagonal
            A[i, i] = 1 / cosh ** 2
        return (tf.constant(A))


    def differentiate_xW_wrt_b(self, W):
        DW = []

        # R^N -> R^T
        # first row -- whole W wrt first output

        # init empty array at first ...
        # output will be matrix (t,Nt)
        N = W.shape[0]
        t = W.shape[1]  # 2
        A = np.zeros((t, N * t))

        # set the correct values
        for i in range(t):
            for j in range(0, N):
                A[i, i * N + j] = 1.0

        print(A)
        return tf.constant(A)

    def differentiate_xW_wrt_x(self, W):

        # R^N -> R^T
        # first row -- whole W wrt first output

        # init empty array at first ...
        # output will be matrix (t,N)
        N = W.shape[0]
        t = W.shape[1]
        A = np.zeros((t, N))

        # set the correct values
        for j in range(t):
            for i in range(N):
                A[j, i] = tf.gather_nd(W, [i, j])  # is this just transposition of W ??

        return (tf.constant(A))

    def predict_for_train(self, input_vector):
        # dense + tanh
        hidden_layer_before_tanh = tf.matmul(input_vector, self._W1)
        hidden_layer_before_tanh = tf.add(hidden_layer_before_tanh, self._b1)

        hidden_layer = tf.nn.tanh(hidden_layer_before_tanh)

        # dense + softmax
        output_layer = tf.matmul(hidden_layer, self._W2)
        output_layer = tf.add(output_layer, self._b2)
        probabilities = tf.math.softmax(output_layer)

        return probabilities, hidden_layer_before_tanh, hidden_layer

    def differentiate_tape(self, y, batch_x):
        with tf.GradientTape() as tape:
            probabilities, hidden_layer_before_tanh, hidden_layer = self.predict_for_train(batch_x)

            # xent
            vynulovane = tf.math.multiply(y, probabilities)
            L = -tf.math.log(tf.math.reduce_sum(vynulovane, axis=1))  # sum over x_i (all but one is 0, so log is on the outside)
            loss = tf.reduce_mean(L)

        variables = [self._W1, self._W2, self._b2, self._b1]

        # Compute the gradient of the loss with respect to variables using
        # backpropagation algorithm via `tape.gradient`
        gradients = tape.gradient(loss, variables)
        return gradients

    def run(self, batch_x, batch_y):
        auto_gradient = self.differentiate_tape(batch_y, batch_x)

        probabilities, hidden_layer_before_tanh, hidden_layer = self.predict_for_train(batch_x)

        N_W1 = self._W1.shape[0]
        t_W1 = self._W1.shape[1]
        manual_gradient_by_W1 = tf.ones([1, N_W1*t_W1], dtype=tf.float64)

        for B in range(batch_x.shape[0]):
            y_row = tf.reshape(tf.gather_nd(batch_y, [B]), [1, -1])
            x_row = tf.reshape(tf.gather_nd(batch_x, [B]), [1, -1])

            probabilities_row = tf.reshape(tf.gather_nd(probabilities, [B]), [1, -1])

            manual_xent_gradient = self.differentiate_xent_wrt_probabilites(y_row, probabilities_row)
            manual_Softmax_gradient = self.differentiate_softmax_wrt_logit(probabilities_row)
            manual_w2_gradient = self.differentiate_xW_wrt_x(self._W2)
            maunal_tanh_gradient = self.differentiate_vect_tanh_wrt_vect(vector_before_tanh=hidden_layer_before_tanh)
            manual_w1_gradient = self.differentiate_xW_by_W(x_row, self._W1)
            manual_gradient_tmp = manual_xent_gradient @ manual_Softmax_gradient @ manual_w2_gradient @ maunal_tanh_gradient @ manual_w1_gradient

            manual_gradient_by_W1 = tf.concat([manual_gradient_by_W1, manual_gradient_tmp], 0)

        manual_gradient_by_W1 = manual_gradient_by_W1[1:, :]
        manual_gradient_by_W1 = tf.math.reduce_mean(manual_gradient_by_W1, axis=0)

        # manual_gradient = tf.math.scalar_mul(1/batch_x.shape[0], manual_gradient)
        manual_gradient_by_W1 = tf.reshape(manual_gradient_by_W1, [t_W1, N_W1])  # because we want to go column-wise
        manual_gradient_by_W1 = tf.transpose(manual_gradient_by_W1)  # because we want to go column-wise
        manual_gradient_by_W1 = tf.cast(manual_gradient_by_W1, tf.float32)



        ############### BY  W2 ###############################

        N_W2 = self._W2.shape[0]
        t_W2 = self._W2.shape[1]
        manual_gradient_by_W2 = tf.ones([1, N_W2, t_W2], dtype=tf.float32)


        for B in range(batch_x.shape[0]):

            y_row = tf.reshape(tf.gather_nd(batch_y, [B]), [1, -1])

            probabilities_row = tf.reshape(tf.gather_nd(probabilities, [B]), [1, -1])

            hidden_layer_row = tf.reshape(tf.gather_nd(hidden_layer, [B]), [1, -1])

            manual_xent_gradient = self.differentiate_xent_wrt_probabilites(y_row, probabilities_row)
            manual_Softmax_gradient = self.differentiate_softmax_wrt_logit(probabilities_row)
            manual_w2_gradient = self.differentiate_xW_by_W(hidden_layer, self._W2)

            manual_gradient_tmp = manual_xent_gradient @ manual_Softmax_gradient @ manual_w2_gradient  # @ maunal_tanh_gradient @ manual_w1_gradient

            # todo -- new way
            # tiling [a b c d] by [2] produces [a b c d a b c d].
            t = self._W2.shape[1]

            print(hidden_layer_row.shape)
            w2_by_w2_matrix = tf.tile(hidden_layer_row, [1, t])
            w2_by_w2_matrix = tf.reshape(w2_by_w2_matrix , [t, -1] )
            w2_by_w2_matrix = tf.transpose(w2_by_w2_matrix)

            xent_soft = ( probabilities_row - y_row )
            xent_soft = tf.tile(xent_soft, [1, hidden_layer_row.shape[1]])
            xent_soft = tf.reshape(xent_soft, [hidden_layer_row.shape[1], -1])

            # print(xent_soft)
            manual_gradient_tmp = tf.multiply(xent_soft, w2_by_w2_matrix)
            manual_gradient_tmp = tf.reshape(manual_gradient_tmp, [1, N_W2, t_W2])
            manual_gradient_by_W2 = tf.concat([manual_gradient_by_W2, manual_gradient_tmp], 0)

        manual_gradient_by_W2 = manual_gradient_by_W2[1:, :]
        manual_gradient_by_W2 = tf.math.reduce_mean(manual_gradient_by_W2, axis=0)

        # manual_gradient = tf.math.scalar_mul(1/batch_x.shape[0], manual_gradient)
        #manual_gradient_by_W2 = tf.reshape(manual_gradient_by_W2, [t_W2, N_W2])  # because we want to go column-wise
        #manual_gradient_by_W2 = tf.transpose(manual_gradient_by_W2)  # because we want to go column-wise
        # manual_gradient_by_W2 = tf.cast(manual_gradient_by_W2, tf.float32)

        ######################################################################################################



        ############### BY  B2 ###############################

        t_b2 = self._W2.shape[1]
        manual_gradient_by_b2 = tf.ones([1, t_b2], dtype=tf.float32)


        for B in range(batch_x.shape[0]):

            y_row = tf.reshape(tf.gather_nd(batch_y, [B]), [1, -1])

            probabilities_row = tf.reshape(tf.gather_nd(probabilities, [B]), [1, -1])

            manual_xent_gradient = self.differentiate_xent_wrt_probabilites(y_row, probabilities_row)
            manual_Softmax_gradient = self.differentiate_softmax_wrt_logit(probabilities_row)

            manual_gradient_tmp = manual_xent_gradient @ manual_Softmax_gradient

            ## todo -- new and simple way
            manual_gradient_tmp = probabilities_row - y_row

            manual_gradient_by_b2 = tf.concat([manual_gradient_by_b2, manual_gradient_tmp], 0)

        manual_gradient_by_b2 = manual_gradient_by_b2[1:, :]
        manual_gradient_by_b2 = tf.math.reduce_mean(manual_gradient_by_b2, axis=0)

        # manual_gradient_by_b2 = tf.cast(manual_gradient_by_b2, tf.float32)

        ######################################################################################################

        ############### BY  B1 ###############################

        t_b1 = self._W1.shape[1]
        manual_gradient_by_b1 = tf.ones([1, t_b1], dtype=tf.float64)

        for B in range(batch_x.shape[0]):
            y_row = tf.reshape(tf.gather_nd(batch_y, [B]), [1, -1])

            probabilities_row = tf.reshape(tf.gather_nd(probabilities, [B]), [1, -1])


            manual_xent_gradient = self.differentiate_xent_wrt_probabilites(y_row, probabilities_row)
            manual_Softmax_gradient = self.differentiate_softmax_wrt_logit(probabilities_row)
            manual_w2_gradient = self.differentiate_xW_wrt_x(self._W2)
            maunal_tanh_gradient = self.differentiate_vect_tanh_wrt_vect(vector_before_tanh=hidden_layer_before_tanh)
            manual_gradient_tmp = manual_xent_gradient @ manual_Softmax_gradient @ manual_w2_gradient @ maunal_tanh_gradient

            manual_gradient_by_b1 = tf.concat([manual_gradient_by_b1, manual_gradient_tmp], 0)

        manual_gradient_by_b1 = manual_gradient_by_b1[1:, :]
        manual_gradient_by_b1 = tf.math.reduce_mean(manual_gradient_by_b1, axis=0)

        manual_gradient_by_b1 = tf.cast(manual_gradient_by_b1, tf.float32)

        ######################################################################################################



        print(auto_gradient[1])

        print("###########")

        print(manual_gradient_by_W2)

        print("###########")
        print("###########")
        print(tf.math.subtract(manual_gradient_by_W2, auto_gradient[1]))

        return manual_gradient_by_W1, manual_gradient_by_W2, manual_gradient_by_b1, manual_gradient_by_b2



if __name__ == "__main__":
    os.environ["TF_CPP_MIN_LOG_LEVEL"] = "3"

    model = DifferTest()
    x = tf.Variable([[7., 8., 9.],  [10.,11.,12.]])  #
    y = tf.Variable([[0., 1.], [0., 1.]])  #
    model.run(x, y)

    # vynulovane = tf.math.multiply(onehot_labels, probabilities)  # elementwise
    # L = -tf.math.log(
    #     tf.math.reduce_sum(vynulovane, axis=1))  # sum over x_i (all but one is 0, so log is on the outside)
    # loss = tf.reduce_mean(L)  # avg over batch examples

