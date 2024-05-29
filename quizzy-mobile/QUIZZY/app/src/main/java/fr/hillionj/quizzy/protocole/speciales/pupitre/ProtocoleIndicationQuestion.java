package fr.hillionj.quizzy.protocole.speciales.pupitre;

import fr.hillionj.quizzy.protocole.Protocole;
import fr.hillionj.quizzy.protocole.TypeProtocole;

@SuppressWarnings({ "SpellCheckingInspection", "unused" })
public class ProtocoleIndicationQuestion extends Protocole
{
    public ProtocoleIndicationQuestion(String trame)
    {
        setTrame(trame);
    }

    @Override
    public String getFormat()
    {
        return "$" + getType().getIndiceType() + ";QUESTION;TEMPS\n";
    }

    @Override
    public TypeProtocole getType()
    {
        return TypeProtocole.INDICATION_QUESTION;
    }

    public int getTempsReponse()
    {
        return toInt(extraireDonnees().get("TEMPS"));
    }

    public int getNumeroQuestion()
    {
        return toInt(extraireDonnees().get("QUESTION"));
    }

    public void genererTrame(int numeroQuestion, int tempsAlloue)
    {
        super.genererTrame(numeroQuestion + "", tempsAlloue + "");
    }
}