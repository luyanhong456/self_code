<?php
/**
 * Auto generated from jtmf_peomanage.proto at 2016-03-16 12:06:43
 *
 * ad.peomanage package
 */

namespace Ad\Peomanage {
/**
 * CDeleteTaskReq message
 */
class CDeleteTaskReq extends \ProtobufMessage
{
    /* Field index constants */
    const SELLERPIN = 1;
    const ID = 2;

    /* @var array Field descriptors */
    protected static $fields = array(
        self::SELLERPIN => array(
            'name' => 'sellerpin',
            'required' => true,
            'type' => 7,
        ),
        self::ID => array(
            'name' => 'id',
            'required' => true,
            'type' => 5,
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
        $this->values[self::ID] = null;
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
     * Sets value of 'id' property
     *
     * @param int $value Property value
     *
     * @return null
     */
    public function setId($value)
    {
        return $this->set(self::ID, $value);
    }

    /**
     * Returns value of 'id' property
     *
     * @return int
     */
    public function getId()
    {
        return $this->get(self::ID);
    }
}
}