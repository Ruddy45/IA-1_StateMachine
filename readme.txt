Modifications apport�es � la version fournie, autres que la cr�ation de l'agent "saoulard" :

Pour le choix de l'�tat initial, il a fallu changer l'�tat de la femme du mineur VisitBathroom et l'�tat EatStew du mineur.
Apr�s avoir ex�cut� leurs �tats, ces deux agents revenaient � leurs �tats pr�c�dents, mais comme c'est l'�tat initial, ils n'existent pas. 
Il a donc fallu les rediriger vers un autre �tat apr�s �a.

Lorsque l'on choisit pour Elsa l'�tat WifesGlobalState comme �tat initial, elle va �tre "sous silence" pendant quelques tours, puisqu'elle
n'a pas d'actions, elle n'effectue qu'un random pour passer � un autre �tat.

L'utilisateur n'a pas le choix pour l'�tat de Bob TalkWithDrunken en �tat initial, car il doit d'abord aller dans le saloon ("QuenchThirst")
pour d�clencher les diff�rents messages entre le mineur et le saoulard.