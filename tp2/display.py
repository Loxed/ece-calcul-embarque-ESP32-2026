"""
display.py

Fonctions d'affichage : comparaison simple avant/après,
et affichage de toutes les étapes intermédiaires d'un pipeline.
Gère aussi bien les images RGB que RGBA (transparence) via
RGBAImage.to_display_array().
"""

import matplotlib.pyplot as plt


def show_result(original, result, title):
    """
    Affiche l'image originale et l'image filtrée, côte à côte.

    original, result : RGBAImage
    """

    plt.figure(figsize=(12, 5))

    plt.subplot(1, 2, 1)
    plt.imshow(original.to_display_array())
    plt.title("Image originale")
    plt.axis("off")

    plt.subplot(1, 2, 2)
    plt.imshow(result.to_display_array())
    plt.title(title)
    plt.axis("off")

    plt.tight_layout()
    plt.show()


def show_pipeline(steps, max_cols=4):
    """
    Affiche toutes les étapes d'un pipeline sur une grille.

    steps : liste de tuples (label, RGBAImage), typiquement le résultat
            de filters.apply_pipeline(...)
    """

    n = len(steps)
    n_cols = min(max_cols, n)
    n_rows = -(-n // n_cols)  # arrondi au-dessus

    plt.figure(figsize=(4 * n_cols, 4 * n_rows))

    for i, (label, image) in enumerate(steps, start=1):
        plt.subplot(n_rows, n_cols, i)
        plt.imshow(image.to_display_array())
        plt.title(label)
        plt.axis("off")

    plt.tight_layout()
    plt.show()