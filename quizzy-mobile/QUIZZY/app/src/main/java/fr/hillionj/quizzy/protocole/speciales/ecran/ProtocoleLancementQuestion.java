package fr.hillionj.quizzy.protocole.speciales.ecran;

import fr.hillionj.quizzy.protocole.Protocole;
import fr.hillionj.quizzy.protocole.TypeProtocole;

@SuppressWarnings({ "SpellCheckingInspection", "unused" })
public class ProtocoleLancementQuestion extends Protocole
{
    @Override
    public String getFormat()
    {
        return "$" + getType().getIndiceType() + "\n";
    }

    @Override
    public TypeProtocole getType()
    {
        return TypeProtocole.DEMARRAGE_QUESTION;
    }

    public void genererTrame() {
        super.genererTrame();
    }
}
