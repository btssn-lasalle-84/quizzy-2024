package fr.hillionj.quizzy.protocole.speciales.ecran;

import fr.hillionj.quizzy.protocole.Protocole;
import fr.hillionj.quizzy.protocole.TypeProtocole;

@SuppressWarnings({ "SpellCheckingInspection", "unused" })
public class ProtocoleAfficherQuestionPrecedente extends Protocole
{
    @Override
    public String getFormat()
    {
        return "$" + getType().getIndiceType() + "\n";
    }

    @Override
    public TypeProtocole getType()
    {
        return TypeProtocole.AFFICHER_QUESTION_PRECEDENTE;
    }

    public void genererTrame()
    {
        super.genererTrame();
    }
}
