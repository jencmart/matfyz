#!/usr/bin/env python3
import argparse
import datetime
import os
import re
import sys

import numpy as np
import tensorflow as tf

from mnist import MNIST


class Model(tf.Module):

    def __init__(self, args):
        self._W1 = tf.Variable(tf.random.normal([MNIST.W * MNIST.H * MNIST.C, args.hidden_layer], stddev=0.1, seed=args.seed), trainable=True)
        self._b1 = tf.Variable(tf.zeros([args.hidden_layer]), trainable=True)

        self._W2 = tf.Variable(tf.random.normal([args.hidden_layer, MNIST.LABELS], stddev=0.1, seed=args.seed))
        self._b2 = tf.Variable(tf.zeros([MNIST.LABELS]))



    # def differentiate_xW_wrt_W(self, x, W):
    #     DW = []
    #
    #     for B in range(self.batch_size):
    #         # R^N -> R^T
    #         # first row -- whole W wrt first output
    #
    #         # init empty array at first ...
    #         # output will be matrix (t,Nt)
    #         N = W.shape[0]
    #         t = W.shape[1]
    #         A = np.zeros((t, N*t))
    #
    #         # set the correct values
    #         for i in range(t):
    #             for j in range(0,  N):
    #                 A[i, i*N+j] = tf.gather_nd(x, [B, j])
    #
    #         DW.append(tf.constant(A))
    #     return DW
    #
    # def differentiate_vect_tanh_wrt_vect(self, vector_before_tanh):
    #
    #     DTH = []
    #
    #     for B in range(self.batch_size):
    #
    #         #  tf.math.divide(tf.constant(1.0), tf.add(tf.constant(1.0), tf.exp(tf.negative(x))))
    #         # R^T -> R^T
    #
    #         # output will be matrix (t,t)
    #         t = vector_before_tanh.shape[1]  # because it is row vector
    #         A = np.zeros((t, t))
    #
    #         for i in range(t):
    #             cosh = np.cosh(tf.gather_nd(vector_before_tanh, [B, i]))  # todo  I think nonzero only on diagonal
    #             A[i, i] = 1 / cosh ** 2
    #
    #         DTH.append(tf.constant(A))
    #     return DTH
    #
    # def differentiate_xW_wrt_x(self, W):
    #
    #     Dx = []
    #     for B in range(self.batch_size):
    #
    #         # R^N -> R^T
    #         # first row -- whole W wrt first output
    #
    #         # init empty array at first ...
    #         # output will be matrix (t,N)
    #         N = W.shape[0]
    #         t = W.shape[1]
    #         A = np.zeros((t, N))
    #
    #         # set the correct values
    #         for j in range(t):
    #             for i in range(N):
    #                 A[j, i] = tf.gather_nd(W, [i, j])  # is this just transposition of W ??
    #
    #         Dx.append(tf.constant(A))
    #     return Dx
    #
    # def differentiate_softmax_wrt_logit(self, probabilities):
    #     DS = []
    #
    #     for B in range(self.batch_size):
    #         # R^T -> R^T
    #         t = probabilities.shape[1]  # because it is row vector
    #
    #         # output will be matrix (t,t)
    #         A = np.zeros((t, t))
    #
    #         for i in range(t):
    #             for j in range(t):
    #                 S_i = tf.gather_nd(probabilities, [B, i])
    #                 S_j = tf.gather_nd(probabilities, [B, j])
    #
    #                 if i == j:
    #                     DjSi = S_i * (1.0 - S_j)
    #                 else:
    #                     DjSi = -1.0 * S_j * S_i
    #                 A[i, j] = DjSi  # podle i-teho outputu .. ano je to spravne
    #
    #         DS.append(tf.constant(A))
    #
    #     return DS
    #
    # def xent_softmax_at_once(self, onehot_y, probabilities):
    #     softmax_xent = []
    #
    #     # vyrobime matici kde na radku vzdy jedno cislo == 1/-p_y
    #     vynulovane = tf.math.multiply(onehot_y, probabilities)
    #     delenec = tf.ones([vynulovane.shape[0], vynulovane.shape[1]], tf.float32)
    #     result = tf.math.scalar_mul(-1.0, tf.divide(delenec, vynulovane))
    #    # result = tf.dtypes.cast(result, dtype=tf.float64)
    #
    #     # remove nan or inf ...
    #     mask = tf.math.is_finite(result)
    #     result = tf.where(mask, result, tf.zeros_like(result))
    #
    #     # now we have matrix ( 1 row == 1 input )
    #     # ted vzdy tu jednu nenulovou hodnu v radku musime vynasobit y-tym radkem v B-tem sloupci
    #     for B in range(self.batch_size):
    #         t = probabilities.shape[1]  # because it is row vector
    #
    #         # output will be row
    #         A = np.zeros((1, t))
    #
    #         for i in range(t):
    #             xent = tf.gather_nd(result, [B, i])
    #             if xent != 0:
    #                 S_i = tf.gather_nd(probabilities, [B, i])
    #
    #                 # tak sme na spravnem "radku"
    #                 for j in range(t):
    #                     S_j = tf.gather_nd(probabilities, [B, j])
    #                     if i == j:
    #                         DjSi_xent = xent * S_i * (1.0 - S_j)
    #                     else:
    #                         DjSi_xent = xent * -1.0 * S_j * S_i
    #                     A[0, j] = DjSi_xent  # podle i-teho outputu .. ano je to spravne
    #                 # row is done - break
    #
    #                 softmax_xent.append(tf.constant(A))
    #                 break
    #
    #     return softmax_xent
    #
    # def differentiate_xent_wrt_probabilites(self, onehot_y, probabilities):
    #     DXENT = []
    #
    #     # vyrobime matici kde na radku vzdy jedno cislo == 1/-p_y
    #     vynulovane = tf.math.multiply(onehot_y, probabilities)
    #     delenec = tf.ones([vynulovane.shape[0], vynulovane.shape[1]], tf.float32)
    #     result = tf.math.scalar_mul(-1.0, tf.divide(delenec, vynulovane))
    #     result = tf.dtypes.cast(result, dtype=tf.float64)
    #
    #     # remove nan or inf ...
    #     mask = tf.math.is_finite(result)
    #     result = tf.where(mask, result, tf.zeros_like(result))
    #
    #     for B in range(self.batch_size):
    #         DXENT.append(tf.reshape(tf.gather_nd(result, [B]), [1, -1]))
    #
    #     return DXENT


    def predict(self, inputs):
        # - start by reshaping the inputs to shape [inputs.shape[0], -1].
        #   The -1 is a wildcard which is computed so that the number
        #   of elements before and after the reshape fits.
        input_layer = tf.reshape(inputs, [inputs.shape[0], -1])  # shape 0 == batch pieces

        # input layer ... multiply the inputs by `self._W1` and then add `self._b1`  apply `tf.nn.tanh`
        hidden_layer = tf.nn.tanh(tf.math.add(tf.linalg.matmul(input_layer, self._W1), self._b1))

        # - multiply the result by `self._W2` and then add `self._b2` apply `tf.nn.softmax` and return the result
        output_layer = tf.nn.softmax(tf.math.add(tf.linalg.matmul(hidden_layer, self._W2), self._b2))
        return output_layer

    def train_epoch(self, dataset):

        for batch in dataset.batches(args.batch_size):

            batch_x = tf.reshape(batch["images"], [batch["images"].shape[0], -1])
            batch_y = tf.one_hot(batch["labels"], MNIST.LABELS)  # 50 x 10

            gradient_by_W1, gradient_by_W2, gradient_by_b1, gradient_by_b2 = self.run(batch_x, batch_y)

            # MANUAL update
            self._W1 = tf.compat.v1.assign_sub(self._W1, tf.math.scalar_mul(args.learning_rate, gradient_by_W1))
            self._W2 = tf.compat.v1.assign_sub(self._W2, tf.math.scalar_mul(args.learning_rate, gradient_by_W2))
            self._b1 = tf.compat.v1.assign_sub(self._b1, tf.math.scalar_mul(args.learning_rate, gradient_by_b1))
            self._b2 = tf.compat.v1.assign_sub(self._b2, tf.math.scalar_mul(args.learning_rate, gradient_by_b2))


    def evaluate(self, dataset):
        # Compute the accuracy of the model prediction
        correct = 0
        for batch in dataset.batches(args.batch_size):
            # Compute the probabilities of the batch images
            output_layer = self.predict(batch["images"])
            probabilities = output_layer
            # Evaluate how many batch examples were predicted
            # correctly and increase `correct` variable accordingly.
            predicted_labels = tf.math.argmax(probabilities, axis=1)
            true_labels = batch["labels"]
            res = predicted_labels - true_labels
            correct += res.shape[0] - tf.math.count_nonzero(res)
        return correct / dataset.size

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

    def run(self, batch_x, batch_y):
        print("starting batch")
        #  auto_gradient = self.differentiate_tape(batch_y, batch_x)
        probabilities, hidden_layer_before_tanh, hidden_layer = self.predict_for_train(batch_x)

        # b2
        t_b2 = self._W2.shape[1]
        manual_gradient_by_b2 = tf.ones([1, t_b2], dtype=tf.float64)

        # W2
        N_W2 = self._W2.shape[0]
        t_W2 = self._W2.shape[1]
        manual_gradient_by_W2 = tf.ones([1, N_W2 * t_W2], dtype=tf.float64)

        # b1
        t_b1 = self._W1.shape[1]
        manual_gradient_by_b1 = tf.ones([1, t_b1], dtype=tf.float64)

        # W1
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
            # for w2 it is different
            manual_w2_gradient_for_w2 = self.differentiate_xW_by_W(hidden_layer, self._W2)
            maunal_tanh_gradient = self.differentiate_vect_tanh_wrt_vect(vector_before_tanh=hidden_layer_before_tanh)
            # manual_w1_gradient_for_w1 = self.differentiate_xW_by_W(x_row, self._W1) ## todo -- tohle je pomaly, ostatni rychle

            # B2
            b2_grad = manual_xent_gradient @ manual_Softmax_gradient  # 10 * (10*10) == 1000
            manual_gradient_by_b2 = tf.concat([manual_gradient_by_b2, b2_grad], 0)

            # W2
            manual_gradient_for_w2_tmp = b2_grad @ manual_w2_gradient_for_w2  # 10 * (10*100) == 10 000
            manual_gradient_by_W2 = tf.concat([manual_gradient_by_W2, manual_gradient_for_w2_tmp], 0)

            # B1
            b1_grad = b2_grad @ manual_w2_gradient @ maunal_tanh_gradient  # 10 * (10*100) * ( 100 * 100 ) == 100 000
            manual_gradient_by_b1 = tf.concat([manual_gradient_by_b1, b1_grad], 0)

            # W1
            # todo -- kvuli tomuto se to vlece -- reseni
            manual_gradient_for_w1_tmp = tf.Variable(tf.ones([1, 0],  dtype=tf.float64))
            for i in range(b1_grad.shape[1]):
                tmp = tf.scalar_mul(tf.gather_nd(b1_grad, [0, i]),  tf.cast(x_row, dtype=tf.float64))
                manual_gradient_for_w1_tmp = tf.concat([manual_gradient_for_w1_tmp, tmp], 1)

            # manual_gradient_for_w1_tmp = b1_grad @ manual_w1_gradient_for_w1
            manual_gradient_by_W1 = tf.concat([manual_gradient_by_W1, manual_gradient_for_w1_tmp], 0)


        # W1
        manual_gradient_by_W1 = manual_gradient_by_W1[1:, :]
        manual_gradient_by_W1 = tf.math.reduce_mean(manual_gradient_by_W1, axis=0)

        manual_gradient_by_W1 = tf.reshape(manual_gradient_by_W1, [t_W1, N_W1])  # because we want to go column-wise
        manual_gradient_by_W1 = tf.transpose(manual_gradient_by_W1)  # because we want to go column-wise
        manual_gradient_by_W1 = tf.cast(manual_gradient_by_W1, tf.float32)

        # W2
        manual_gradient_by_W2 = manual_gradient_by_W2[1:, :]
        manual_gradient_by_W2 = tf.math.reduce_mean(manual_gradient_by_W2, axis=0)

        manual_gradient_by_W2 = tf.reshape(manual_gradient_by_W2, [t_W2, N_W2])  # because we want to go column-wise
        manual_gradient_by_W2 = tf.transpose(manual_gradient_by_W2)  # because we want to go column-wise
        manual_gradient_by_W2 = tf.cast(manual_gradient_by_W2, tf.float32)

        # B2
        manual_gradient_by_b2 = manual_gradient_by_b2[1:, :]
        manual_gradient_by_b2 = tf.math.reduce_mean(manual_gradient_by_b2, axis=0)

        manual_gradient_by_b2 = tf.cast(manual_gradient_by_b2, tf.float32)

        # B1
        manual_gradient_by_b1 = manual_gradient_by_b1[1:, :]
        manual_gradient_by_b1 = tf.math.reduce_mean(manual_gradient_by_b1, axis=0)

        manual_gradient_by_b1 = tf.cast(manual_gradient_by_b1, tf.float32)


        ######################################################################################################


        # print(auto_gradient[3])
        #
        # print("###########")
        #
        # print(manual_gradient_by_b1)
        #
        # print("###########")
        # print("###########")
        # print(tf.math.subtract(manual_gradient_by_W1, auto_gradient[0]))
        print("batch finished")

        return manual_gradient_by_W1, manual_gradient_by_W2, manual_gradient_by_b1, manual_gradient_by_b2

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
            cosh = np.cosh(tf.gather_nd(vector_before_tanh, [0, i]))
            A[i, i] = 1 / cosh ** 2
        return tf.constant(A)

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


if __name__ == "__main__":
    # Parse arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("--batch_size", default=50, type=int, help="Batch size.")
    parser.add_argument("--epochs", default=10, type=int, help="Number of epochs.")
    parser.add_argument("--hidden_layer", default=100, type=int, help="Size of the hidden layer.")
    parser.add_argument("--learning_rate", default=0.2, type=float, help="Learning rate.")
    parser.add_argument("--seed", default=42, type=int, help="Random seed.")
    parser.add_argument("--threads", default=1, type=int, help="Maximum number of threads to use.")
    parser.add_argument("--verbose", default=False, action="store_true", help="Verbose TF logging.")
    args = parser.parse_args([] if "__file__" not in globals() else None)

    # Fix random seeds and threads
    np.random.seed(args.seed)
    tf.random.set_seed(args.seed)
    tf.config.threading.set_inter_op_parallelism_threads(args.threads)
    tf.config.threading.set_intra_op_parallelism_threads(args.threads)

    # Report only errors by default
    if not args.verbose:
        os.environ["TF_CPP_MIN_LOG_LEVEL"] = "3"

    # Create logdir name
    args.logdir = os.path.join("logs", "{}-{}-{}".format(
        os.path.basename(globals().get("__file__", "notebook")),
        datetime.datetime.now().strftime("%Y-%m-%d_%H%M%S"),
        ",".join(("{}={}".format(re.sub("(.)[^_]*_?", r"\1", key), value) for key, value in sorted(vars(args).items())))
    ))

    # Load data
    mnist = MNIST()

    # Create the TensorBoard writer
    writer = tf.summary.create_file_writer(args.logdir, flush_millis=10*1000)

    # Create the model
    model = Model(args)

    # run in 10 times !!! oh my god !!!
    for epoch in range(args.epochs):
        # Run the `train_epoch` with `mnist.train` dataset
        model.train_epoch(mnist.train)

        # (sgd_backpropagation): Evaluate the dev data using `evaluate` on `mnist.dev` dataset
        accuracy = model.evaluate(mnist.dev)

        print("Dev accuracy after epoch {} is {:.2f}".format(epoch + 1, 100 * accuracy), flush=True)
        with writer.as_default():
            tf.summary.scalar("dev/accuracy", 100 * accuracy, step=epoch + 1)

    # (sgd_backpropagation): Evaluate the test data using `evaluate` on `mnist.test` dataset
    accuracy = model.evaluate(mnist.test)
    print("Test accuracy after epoch {} is {:.2f}".format(epoch + 1, 100 * accuracy), flush=True)
    with writer.as_default():
        tf.summary.scalar("test/accuracy", 100 * accuracy, step=epoch + 1)

    # Save the test accuracy in percents rounded to two decimal places.
    with open("sgd_manual.out", "w") as out_file:
        print("{:.2f}".format(100 * accuracy), file=out_file)