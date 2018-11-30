<?php
/**
 * Auto generated from jtmf_peomanage.proto at 2016-03-16 12:06:43
 *
 * ad.peomanage package
 */

namespace Ad\Peomanage {
/**
 * CQueryTaskListReq message
 */
class CQueryTaskListReq extends \ProtobufMessage
{
    /* Field index constants */
    const SELLERPIN = 1;
    const NAME = 2;
    const TASKDESC = 3;

    /* @var array Field descriptors */
    protected static $fields = array(
        self::SELLERPIN => array(
            'name' => 'sellerpin',
            'required' => true,
            'type' => 7,
        ),
        self::NAME => array(
            'name' => 'name',
            'required' => false,
            'type' => 7,
        ),
        self::TASKDESC => array(
            'name' => 'taskdesc',
            'required' => false,
            'type' => 7,
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
        $this->values[self::SELLERPIN] = null;
        $this->values[self::NAME] = null;
        $this->values[self::TASKDESC] = null;
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
     * Sets value of 'name' property
     *
     * @param string $value Property value
     *
     * @return null
     */
    public function setName($value)
    {
        return $this->set(self::NAME, $value);
    }

    /**
     * Returns value of 'name' property
     *
     * @return string
     */
    public function getName()
    {
        return $this->get(self::NAME);
    }

    /**
     * Sets value of 'taskdesc' property
     *
     * @param string $value Property value
     *
     * @return null
     */
    public function setTaskdesc($value)
    {
        return $this->set(self::TASKDESC, $value);
    }

    /**
     * Returns value of 'taskdesc' property
     *
     * @return string
     */
    public function getTaskdesc()
    {
        return $this->get(self::TASKDESC);
    }
}
}