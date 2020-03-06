Modifications apportées à la version fournie, autres que la création de l'agent "saoulard" :

Pour le choix de l'état initial, il a fallu changer l'état de la femme du mineur VisitBathroom et l'état EatStew du mineur.
Après avoir exécuté leurs états, ces deux agents revenaient à leurs états précédents, mais comme c'est l'état initial, ils n'existent pas. 
Il a donc fallu les rediriger vers un autre état après ça.

Lorsque l'on choisit pour Elsa l'état WifesGlobalState comme état initial, elle va être "sous silence" pendant quelques tours, puisqu'elle
n'a pas d'actions, elle n'effectue qu'un random pour passer à un autre état.

L'utilisateur n'a pas le choix pour l'état de Bob TalkWithDrunken en état initial, car il doit d'abord aller dans le saloon ("QuenchThirst")
pour déclencher les différents messages entre le mineur et le saoulard.