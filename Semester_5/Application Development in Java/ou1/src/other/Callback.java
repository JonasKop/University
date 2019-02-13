package other;

/**
 * @author Jonas Sjödin
 * @version 1.0
 * Email: id16jsn@cs.umu.se
 * Date: December 2018
 * *****************************************************************************
 * Callback class which contains a method that takes data and performs an action
 * by using that data.
 *
 * @param <Type> data type that should be used in the callback
 */
public interface Callback<Type> {

    /**
     * Callback method that takes data as an argument and does something and
     * performs an action with/of that data.
     *
     * @param data the data that should be used in the callback
     */
    void callBack(Type data);
}
