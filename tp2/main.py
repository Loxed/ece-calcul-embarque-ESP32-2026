"""
@author: leopoldrombaut

main.py

Point d'entrée : charge une image (RGB ou RGBA/transparente),
applique une ou plusieurs opérations à la suite, affiche toutes
les étapes intermédiaires, et peut sauvegarder le résultat final
en conservant la transparence.
"""

from operations import load_image, save_image
from filters import apply_pipeline, OPERATIONS
from display import show_pipeline, show_result


def run(image_path, steps, show_all_steps=True, save_path=None):
    """
    image_path     : chemin vers l'image (PNG transparent supporté)
    steps          : liste d'opérations, ex: ["flou", "netteté"]
                    ou [("red_shift", {"amount": 80}), "edges"]
    show_all_steps : affiche chaque étape si True, sinon juste avant/après
    save_path      : si fourni, sauvegarde le résultat final
                    (utiliser une extension .png pour garder l'alpha)
    """

    image = load_image(image_path)

    if image.has_alpha:
        print("Transparence détectée : le canal alpha sera préservé.")

    results = apply_pipeline(image, steps)

    if show_all_steps:
        show_pipeline(results)
    else:
        # results[0] = originale, results[-1] = résultat final
        _, original = results[0]
        last_label, final = results[-1]
        show_result(original, final, last_label)

    if save_path is not None:
        _, final_image = results[-1]
        save_image(save_path, final_image)
        print(f"Résultat sauvegardé : {save_path}")

    return results


# ============================================================
# EXEMPLE
# ============================================================

if __name__ == "__main__":

    demo_image_path = "test.png"

    # Une seule opération, comme le script d'origine
    # run(demo_image_path, ["flou"])

    # Plusieurs opérations enchaînées, toutes les étapes affichées,
    # et sauvegarde du résultat final en conservant la transparence
    run(
        demo_image_path,
        ["edges"],
        show_all_steps=True,
        save_path="kirby_result.png"
    )

    # Pour voir juste les opérations disponibles :
    # print(sorted(OPERATIONS))