"""
filters.py

Fonctions de traitement d'image : convolution par kernel,
décalage de canaux RGB, et registre d'opérations disponibles.

Les kernels sont appliqués en tenant compte de la transparence :
si l'image a un canal alpha, on convolue en alpha "prémultiplié"
pour éviter les franges sombres autour des zones transparentes
(les pixels RGB sous une zone 100% transparente sont souvent
arbitraires/noirs dans un PNG, donc un flou naïf les mélangerait
aux pixels visibles voisins). L'alpha est lui-même flouté avec le
même kernel, ce qui adoucit naturellement les bords de la forme.
"""

import cv2
import numpy as np

from kernels import KERNELS
from operations import RGBAImage


# ============================================================
# CONVOLUTION (avec prise en compte de l'alpha)
# ============================================================

def _convolve(channels, kernel):
    """Convolution brute (filter2D). Fonctionne sur 1, 3 ou 4 canaux."""

    return cv2.filter2D(
        channels,
        ddepth=-1,
        kernel=kernel,
        borderType=cv2.BORDER_DEFAULT
    )


def apply_kernel(image: RGBAImage, kernel):
    """
    Applique un kernel de convolution à une RGBAImage.

    - Sans alpha : convolution directe sur le RGB, comme avant.
    - Avec alpha : convolution en alpha prémultiplié pour ne pas
      laisser les couleurs "invisibles" polluer les bords visibles,
      puis on repasse en alpha normal (division par l'alpha flouté).
    """

    if not image.has_alpha:
        result_rgb = _convolve(image.rgb, kernel)
        return RGBAImage(result_rgb, alpha=None)

    alpha_f = image.alpha.astype(np.float32) / 255.0
    rgb_f = image.rgb.astype(np.float32)

    # Prémultiplication : couleur pondérée par l'opacité
    premultiplied = rgb_f * alpha_f[:, :, None]

    blurred_premultiplied = _convolve(premultiplied, kernel)
    blurred_alpha = _convolve(alpha_f, kernel)

    # Évite une division par zéro dans les zones 100% transparentes
    safe_alpha = np.clip(blurred_alpha, 1e-6, None)

    result_rgb = blurred_premultiplied / safe_alpha[:, :, None]
    result_rgb = np.clip(result_rgb, 0, 255).astype(np.uint8)

    result_alpha = np.clip(blurred_alpha * 255.0, 0, 255).astype(np.uint8)

    return RGBAImage(result_rgb, alpha=result_alpha)


# ============================================================
# RGB SHIFT (ne touche jamais à l'alpha)
# ============================================================

def red_shift(image: RGBAImage, amount=50):
    """Renforce le canal rouge. La transparence n'est pas modifiée."""

    rgb = image.rgb.astype(np.int16)
    rgb[:, :, 0] = np.clip(rgb[:, :, 0] + amount, 0, 255)

    return RGBAImage(rgb.astype(np.uint8), alpha=image.alpha)


def blue_shift(image: RGBAImage, amount=50):
    """Renforce le canal bleu. La transparence n'est pas modifiée."""

    rgb = image.rgb.astype(np.int16)
    rgb[:, :, 2] = np.clip(rgb[:, :, 2] + amount, 0, 255)

    return RGBAImage(rgb.astype(np.uint8), alpha=image.alpha)


# ============================================================
# REGISTRE DES OPÉRATIONS
# ============================================================
# Chaque opération est une fonction (RGBAImage, **kwargs) -> RGBAImage.
# Ça permet d'enchaîner n'importe quel mélange de kernels et de shifts
# sans dupliquer de logique, et la transparence suit automatiquement.

def _make_kernel_op(kernel_name):
    """Construit une fonction d'opération à partir d'un nom de kernel."""

    def op(image, **kwargs):
        return apply_kernel(image, KERNELS[kernel_name])

    return op


OPERATIONS = {name: _make_kernel_op(name) for name in KERNELS}

OPERATIONS["red_shift"] = red_shift
OPERATIONS["blue_shift"] = blue_shift


def apply_operation(image, name, **kwargs):
    """
    Applique une seule opération (kernel ou shift) par son nom.
    """

    if name not in OPERATIONS:
        raise ValueError(
            f"Opération inconnue : {name}\n"
            f"Disponibles : {', '.join(OPERATIONS)}"
        )

    return OPERATIONS[name](image, **kwargs)


def apply_pipeline(image, steps):
    """
    Applique une suite d'opérations sur une RGBAImage, dans l'ordre,
    et renvoie toutes les étapes intermédiaires (transparence comprise).

    steps : liste d'éléments, chacun étant soit :
              - un nom d'opération (str), ex: "flou"
              - un tuple (nom, kwargs), ex: ("red_shift", {"amount": 80})

    Retourne une liste de tuples (label, RGBAImage), en commençant
    par ("originale", image_de_depart).
    """

    results = [("originale", image)]
    current = image

    for step in steps:
        if isinstance(step, tuple):
            name, kwargs = step
        else:
            name, kwargs = step, {}

        current = apply_operation(current, name, **kwargs)
        results.append((name, current))

    return results