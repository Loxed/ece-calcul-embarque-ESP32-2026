#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
kernels.py

Définition de tous les kernels de convolution disponibles.
"""

import numpy as np


KERNELS = {

    # --------------------------------------------------------
    # Flou
    # --------------------------------------------------------

    "flou": np.array([
        [1, 1, 1],
        [1, 1, 1],
        [1, 1, 1]
    ], dtype=np.float32) / 9.0,

    "flou_fort": np.ones((5, 5), dtype=np.float32) / 25.0,


    # --------------------------------------------------------
    # Netteté / "deflouter"
    # --------------------------------------------------------

    "netteté": np.array([
        [ 0, -1,  0],
        [-1,  5, -1],
        [ 0, -1,  0]
    ], dtype=np.float32),

    "netteté_forte": np.array([
        [-1, -1, -1],
        [-1,  9, -1],
        [-1, -1, -1]
    ], dtype=np.float32),


    # --------------------------------------------------------
    # Edges
    # --------------------------------------------------------

    "edges": np.array([
        [-1, -1, -1],
        [-1,  8, -1],
        [-1, -1, -1]
    ], dtype=np.float32),

    "horizontal": np.array([
        [-1, -1, -1],
        [ 0,  0,  0],
        [ 1,  1,  1]
    ], dtype=np.float32),

    "vertical": np.array([
        [-1,  0,  1],
        [-1,  0,  1],
        [-1,  0,  1]
    ], dtype=np.float32),

    "sobel_horizontal": np.array([
        [-1, -2, -1],
        [ 0,  0,  0],
        [ 1,  2,  1]
    ], dtype=np.float32),

    "sobel_vertical": np.array([
        [-1,  0,  1],
        [-2,  0,  2],
        [-1,  0,  1]
    ], dtype=np.float32),

    # Sobel vertical normalisé (divisé par 4), valeurs entre -0.5 et 0.5
    "sobel_vertical_norm": np.array([
        [-0.25, 0.00,  0.25],
        [-0.50, 0.00,  0.50],
        [-0.25, 0.00,  0.25]
    ], dtype=np.float32),

    "diagonale_1": np.array([
        [-1, -1,  2],
        [-1,  2, -1],
        [ 2, -1, -1]
    ], dtype=np.float32),

    "diagonale_2": np.array([
        [ 2, -1, -1],
        [-1,  2, -1],
        [-1, -1,  2]
    ], dtype=np.float32),
}