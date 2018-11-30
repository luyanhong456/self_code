<?php
/**
 * Auto generated from jtmf_peomanage.proto at 2016-03-16 12:06:43
 *
 * ad.peomanage package
 */

namespace Ad\Peomanage {
/**
 * CQueryTaskListResp message
 */
class CQueryTaskListResp extends \ProtobufMessage
{
    /* Field index constants */
    const CODE = 1;
    const MSG = 2;
    const SELLERPIN = 3;
    const VTASKDATA = 4;

    /* @var array Field descriptors */
    protected static $fields = array(
        self::CODE => array(
            'name' => 'code',
            'required' => true,
            'type' => 5,
        ),
        self::MSG => array(
            'name' => 'msg',
            'required' => true,
            'type' => 7,
        ),
        self::SELLERPIN => array(
            'name' => 'sellerpin',
            'required' => true,
            'type' => 7,
        ),
        self::VTASKDATA => array(
            'name' => 'vTaskData',
            'repeated' => true,
            'type' => '\Ad\Peomanage\CQueryTaskItem'
        ),
    );

    /**
     * Constructs new message container and clears its internal state
     *
     * @return null
     */
    public function __construct()
    {
        $this->reset();
    }

    /**
     * Clears message values and sets default ones
     *
     * @return null
     */
    public function reset()
    {
        $this->values[self::CODE] = null;
        $this->values[self::MSG] = null;
        $this->values[self::SELLERPIN] = null;
        $this->values[self::VTASKDATA] = array();
    }

    /**
     * Returns field descriptors
     *
     * @return array
     */
    public function fields()
    {
        return self::$fields;
    }

    /**
     * Sets value of 'code' property
     *
     * @param int $value Property value
     *
     * @return null
     */
    public function setCode($value)
    {
        return $this->set(self::CODE, $value);
    }

    /**
     * Returns value of 'code' property
     *
     * @return int
     */
    public function getCode()
    {
        return $this->get(self::CODE);
    }

    /**
     * Sets value of 'msg' property
     *
     * @param string $value Property value
     *
     * @return null
     */
    public function setMsg($value)
    {
        return $this->set(self::MSG, $value);
    }

    /**
     * Returns value of 'msg' property
     *
     * @return string
     */
    public function getMsg()
    {
        return $this->get(self::MSG);
    }

    /**
     * Sets value of 'sellerpin' property
     *
     * @param string $value Property value
     *
     * @return null
     */
    public function setSellerpin($value)
    {
        return $this->set(self::SELLERPIN, $value);
    }

    /**
     * Returns value of 'sellerpin' property
     *
     * @return string
     */
    public function getSellerpin()
    {
        return $this->get(self::SELLERPIN);
    }

    /**
     * Appends value to 'vTaskData' list
     *
     * @param \Ad\Peomanage\CQueryTaskItem $value Value to append
     *
     * @return null
     */
    public function appendVTaskData(\Ad\Peomanage\CQueryTaskItem $value)
    {
        return $this->append(self::VTASKDATA, $value);
    }

    /**
     * Clears 'vTaskData' list
     *
     * @return null
     */
    public function clearVTaskData()
    {
        return $this->clear(self::VTASKDATA);
    }

    /**
     * Returns 'vTaskData' list
     *
     * @return \Ad\Peomanage\CQueryTaskItem[]
     */
    public function getVTaskData()
    {
        return $this->get(self::VTASKDATA);
    }

    /**
     * Returns 'vTaskData' iterator
     *
     * @return ArrayIterator
     */
    public function getVTaskDataIterator()
    {
        return new \ArrayIterator($this->get(self::VTASKDATA));
    }

    /**
     * Returns element from 'vTaskData' list at given offset
     *
     * @param int $offset Position in list
     *
     * @return \Ad\Peomanage\CQueryTaskItem
     */
    public function getVTaskDataAt($offset)
    {
        return $this->get(self::VTASKDATA, $offset);
    }

    /**
     * Returns count of 'vTaskData' list
     *
     * @return int
     */
    public function getVTaskDataCount()
    {
        return $this->count(self::VTASKDATA);
    }
}
}