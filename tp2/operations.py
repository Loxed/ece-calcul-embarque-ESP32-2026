"""
operations.py

Chargement et sauvegarde d'images, avec prise en compte de la
transparence (canal alpha) des PNG.
"""

import cv2
import numpy as np


class RGBAImage:
    """
    Représente une image en RGB avec un canal alpha optionnel.

    rgb   : tableau (H, W, 3) uint8
    alpha : tableau (H, W) uint8, ou None si l'image n'a pas de transparence
    """

    def __init__(self, rgb, alpha=None):
        self.rgb = rgb
        self.alpha = alpha

    @property
    def has_alpha(self):
        return self.alpha is not None

    def to_display_array(self):
        """
        Renvoie un tableau prêt pour matplotlib : RGB si pas d'alpha,
        RGBA sinon.
        """

        if self.has_alpha:
            return np.dstack([self.rgb, self.alpha])

        return self.rgb

    def copy(self):
        alpha = None if self.alpha is None else self.alpha.copy()
        return RGBAImage(self.rgb.copy(), alpha)


def load_image(image_path):
    """
    Charge une image et détecte automatiquement si elle a un canal
    alpha (transparence). Renvoie un RGBAImage.
    """

    # IMREAD_UNCHANGED : nécessaire pour préserver le canal alpha,
    # contrairement à IMREAD_COLOR qui l'ignore.
    raw = cv2.imread(image_path, cv2.IMREAD_UNCHANGED)

    if raw is None:
        raise FileNotFoundError(f"Impossible de charger l'image : {image_path}")

    if raw.ndim == 2:
        # image en niveaux de gris, pas de transparence
        rgb = cv2.cvtColor(raw, cv2.COLOR_GRAY2RGB)
        return RGBAImage(rgb, alpha=None)

    if raw.shape[2] == 4:
        # PNG avec transparence : BGRA -> RGB + alpha séparé
        bgr = raw[:, :, :3]
        alpha = raw[:, :, 3]
        rgb = cv2.cvtColor(bgr, cv2.COLOR_BGR2RGB)
        return RGBAImage(rgb, alpha=alpha)

    # BGR classique, pas de transparence
    rgb = cv2.cvtColor(raw, cv2.COLOR_BGR2RGB)
    return RGBAImage(rgb, alpha=None)


def save_image(image_path, image: RGBAImage):
    """
    Sauvegarde un RGBAImage sur disque, en conservant la transparence
    si elle existe. Utiliser une extension .png pour garder l'alpha
    (le jpg ne supporte pas la transparence et l'ignorerait).
    """

    bgr = cv2.cvtColor(image.rgb, cv2.COLOR_RGB2BGR)

    if image.has_alpha:
        bgra = np.dstack([bgr, image.alpha])
        cv2.imwrite(image_path, bgra)
    else:
        cv2.imwrite(image_path, bgr)